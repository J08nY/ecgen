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
	struct pari_thread *pthread = (struct pari_thread *)arg;
	// TODO: This is dodgy ... as its really a GEN (so a long*)
	thread_t *thread = (thread_t *)pari_thread_start(pthread);
	random_init();
	arg_t *invalid_argss[OFFSET_END];

	curve_t *invalid = curve_new();
	invalid->field = gcopy(thread->original_curve->field);
	invalid->a = gcopy(thread->original_curve->a);

	while (*thread->generated < thread->nprimes) {
		pari_sp btop = avma;
		exhaustive_gen(invalid, thread->cfg, thread->gens, NULL, OFFSET_B,
		               OFFSET_GENERATORS);
		size_t ndivides = 0;
		for (size_t i = thread->nprimes; i-- > 0;) {
			if (dvdis(invalid->order, thread->primes[i])) {
				// whoo we have a new invalid curve
				ndivides++;
			}
		}
#ifdef DEBUG
		printf("ndivides = %lu\n", ndivides);
#endif
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
#ifdef DEBUG
			printf("nfree = %lu\n", nfree);
#endif
			pthread_mutex_unlock(thread->mutex_state);

			if (nfree > 0) {
				arg_t prime_divisors = {primes, nprimes};
				invalid_argss[OFFSET_POINTS] = &prime_divisors;
				exhaustive_gen(invalid, thread->cfg, thread->gens,
				               invalid_argss, OFFSET_GENERATORS, OFFSET_END);

				pthread_mutex_lock(thread->mutex_curves);
				pthread_mutex_lock(thread->mutex_state);
				size_t count = 0;
				for (size_t i = thread->nprimes; i-- > 0;) {
					if (count < nprimes && primes[count] == thread->primes[i]) {
#ifdef DEBUG
						printf("[i] = %lu, prime = %lu\n", i, primes[count]);
						printf("state = %i\n", thread->states[i]);
#endif
						thread->states[i] = STATE_GENERATED;
						thread->curves[i] = curve_new_copy(invalid);

						output_o(thread->curves[i], thread->cfg);
						count++;
					}
				}
#ifdef DEBUG
				printf("count = %lu, generated = %lu\n", count,
				       *(thread->generated));
#endif
				*(thread->generated) += count;
#ifdef DEBUG
				printf("generated = %lu\n", *(thread->generated));
#endif
				// pthread_cond_signal(thread->cond_generated);
				pthread_mutex_unlock(thread->mutex_state);
				pthread_mutex_unlock(thread->mutex_curves);

				invalid = curve_new();
				invalid->field = gcopy(thread->original_curve->field);
				invalid->a = gcopy(thread->original_curve->a);
			} else {
				obj_free(invalid->curve);  // necessary to free the ellinit
				avma = btop;
			}
		} else {
			obj_free(invalid->curve);  // necessary to free the ellinit
			avma = btop;
		}
	}
	curve_free(&invalid);

	pari_thread_close();
	return NULL;
}