/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */

#include "invalid_thread.h"
#include "gen/curve.h"
#include "gen/gens.h"
#include "gen/point.h"
#include "obj/curve.h"
#include "util/random.h"
#include "util/timeout.h"

static size_t invalid_get_generated(thread_t *thread) {
	pthread_mutex_lock(thread->mutex_state);
	size_t result = *thread->generated;
	pthread_mutex_unlock(thread->mutex_state);
	return result;
}

void *invalid_thread(void *arg) {
	thread_t *thread = (thread_t *)arg;
	pari_thread_start(thread->pari_thread);
	random_init();
	timeout_thread_init();
	arg_t *invalid_argss[OFFSET_END] = {NULL};
	exhaustive_t invalid_setup = {.generators = thread->setup->generators,
	                              .validators = thread->setup->validators,
	                              .gen_argss = invalid_argss,
	                              .unrolls = thread->setup->unrolls};

	curve_t *invalid = curve_new();
	invalid->field = gcopy(thread->original_curve->field);
	invalid->a = gcopy(thread->original_curve->a);

	while (invalid_get_generated(thread) < thread->nprimes) {
		pari_sp btop = avma;
		exhaustive_gen(invalid, thread->setup, OFFSET_B, OFFSET_GENERATORS);
		size_t ndivides = 0;
		size_t nfree = 0;
		for (size_t i = thread->nprimes; i-- > 0;) {
			if (dvdis(invalid->order, thread->primes[i])) {
				// whoo we have a new invalid curve
				ndivides++;
			}
		}

		if (ndivides > 0 &&
		    exhaustive_gen_retry(invalid, &invalid_setup, OFFSET_GENERATORS,
		                         OFFSET_POINTS, 1)) {
			pthread_mutex_lock(thread->mutex_state);
			pari_ulong primes[ndivides];
			size_t nprimes = 0;
			for (size_t i = thread->nprimes; i-- > 0;) {
				if (dvdis(invalid->order, thread->primes[i]) &&
				    thread->states[i] == STATE_FREE) {
					thread->states[i] = STATE_GENERATING;
					primes[nprimes++] = thread->primes[i];
					nfree++;
				}
			}
			pthread_mutex_unlock(thread->mutex_state);

			if (nfree > 0) {
				arg_t prime_divisors = {primes, nprimes};
				invalid_argss[OFFSET_POINTS] = &prime_divisors;
				exhaustive_gen(invalid, &invalid_setup, OFFSET_POINTS,
				               OFFSET_END);

				pthread_mutex_lock(thread->mutex_state);
				size_t count = 0;
				for (size_t i = thread->nprimes; i-- > 0;) {
					if (count < nprimes && primes[count] == thread->primes[i]) {
						thread->states[i] = STATE_GENERATED;
						thread->curves[i] = curve_new_copy(invalid);
						count++;
					}
				}
				*(thread->generated) += count;
				pthread_cond_signal(thread->cond_generated);
				pthread_mutex_unlock(thread->mutex_state);
			}
		}

		points_unroll(invalid, avma, btop);
		gens_unroll(invalid, avma, btop);
		curve_unroll(invalid, avma, btop);
		if (nfree == 0) avma = btop;
	}
	curve_free(&invalid);

	pari_thread_close();
	timeout_thread_quit();
	return NULL;
}
