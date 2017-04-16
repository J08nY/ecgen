/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include "invalid_thread.h"
#include "exhaustive/exhaustive.h"
#include "io/output.h"
#include "math/curve.h"
#include "math/random.h"

void *invalid_thread(void *arg) {
	thread_t *thread = (thread_t *)arg;
	pari_thread_start(thread->pari_thread);
	random_init();
	arg_t *invalid_argss[OFFSET_END];

	curve_t *invalid = curve_new();
	invalid->field = gcopy(thread->original_curve->field);
	invalid->a = gcopy(thread->original_curve->a);

	while (*thread->generated < thread->nprimes) {
		pari_sp btop = avma;
		exhaustive_gen(invalid, thread->cfg, thread->gens, NULL,
		               thread->unrolls, OFFSET_B, OFFSET_GENERATORS);
		size_t ndivides = 0;
		for (size_t i = thread->nprimes; i-- > 0;) {
			if (dvdis(invalid->order, thread->primes[i])) {
				// whoo we have a new invalid curve
				ndivides++;
			}
		}
		debug("ndivides = %lu\n", ndivides);
		if (ndivides > 0) {
			pthread_mutex_lock(thread->mutex_state);
			size_t nfree = 0;
			// can be up to ndivides, but also lower...
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
			debug("nfree = %lu\n", nfree);
			pthread_mutex_unlock(thread->mutex_state);

			if (nfree > 0) {
				arg_t prime_divisors = {primes, nprimes};
				invalid_argss[OFFSET_POINTS] = &prime_divisors;
				exhaustive_gen(invalid, thread->cfg, thread->gens,
				               invalid_argss, thread->unrolls,
				               OFFSET_GENERATORS, OFFSET_END);

				pthread_mutex_lock(thread->mutex_state);
				size_t count = 0;
				for (size_t i = thread->nprimes; i-- > 0;) {
					if (count < nprimes && primes[count] == thread->primes[i]) {
						debug("[i] = %lu, prime = %lu\n", i, primes[count]);
						debug("state = %i\n", thread->states[i]);
						thread->states[i] = STATE_GENERATED;
						thread->curves[i] = curve_new_copy(invalid);
						count++;
					}
				}
				debug("count = %lu, generated = %lu\n", count,
				      *(thread->generated));
				*(thread->generated) += count;
				debug("generated = %lu\n", *(thread->generated));
				pthread_cond_signal(thread->cond_generated);
				pthread_mutex_unlock(thread->mutex_state);

				invalid = curve_new();
				invalid->field = gcopy(thread->original_curve->field);
				invalid->a = gcopy(thread->original_curve->a);
			} else {
				curve_unroll(invalid, thread->cfg, avma, btop);
				avma = btop;
			}
		} else {
			curve_unroll(invalid, thread->cfg, avma, btop);
			avma = btop;
		}
	}
	curve_free(&invalid);

	pari_thread_close();
	return NULL;
}
