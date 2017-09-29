/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "invalid.h"
#include "exhaustive/check.h"
#include "exhaustive/exhaustive.h"
#include "gen/curve.h"
#include "gen/equation.h"
#include "gen/field.h"
#include "gen/gens.h"
#include "gen/order.h"
#include "gen/point.h"
#include "invalid_thread.h"
#include "io/output.h"
#include "util/memory.h"

static void invalid_original_ginit(gen_f *generators, const config_t *cfg) {
	generators[OFFSET_SEED] = &gen_skip;
	if (cfg->random) {
		generators[OFFSET_FIELD] = &field_gen_random;
		generators[OFFSET_A] = &a_gen_random;
		generators[OFFSET_B] = &b_gen_random;
	} else {
		generators[OFFSET_FIELD] = &field_gen_input;
		generators[OFFSET_A] = &a_gen_input;
		generators[OFFSET_B] = &b_gen_input;
	}
	generators[OFFSET_GENERATORS] = &gens_gen_any;
	generators[OFFSET_CURVE] = &curve_gen_any;
	generators[OFFSET_ORDER] = &order_gen_any;
}

static void invalid_invalid_ginit(gen_f *generators, const config_t *cfg) {
	generators[OFFSET_SEED] = &gen_skip;
	generators[OFFSET_FIELD] = &gen_skip;
	generators[OFFSET_A] = &gen_skip;
	generators[OFFSET_B] = &b_gen_random;
	generators[OFFSET_CURVE] = &curve_gen_any;
	generators[OFFSET_ORDER] = &order_gen_any;
	if (cfg->unique) {
		generators[OFFSET_GENERATORS] = &gens_gen_one;
	} else {
		generators[OFFSET_GENERATORS] = &gens_gen_any;
	}
	generators[OFFSET_POINTS] = &points_gen_trial;
}

static void invalid_cinit(check_t **validators, const config_t *cfg) {
	check_t *curve_check = check_new(curve_check_nonzero, NULL);
	validators[OFFSET_CURVE] = curve_check;
}

static size_t invalid_primes(GEN order, pari_ulong **primes) {
	pari_sp ltop = avma;

	GEN bound = sqri(order);
	GEN product = gen_1;
	pari_ulong last = 1;
	size_t nprimes = 0;

	size_t size = 10;
	*primes = try_calloc(size * sizeof(pari_ulong));

	while (cmpii(bound, product) >= 0) {
		product = mulis(product, last);
		(*primes)[nprimes] = unextprime(last + 1);
		last = (*primes)[nprimes];
		nprimes++;
		if (nprimes == size) {
			pari_ulong *new_primes =
			    try_realloc(*primes, size * 2 * sizeof(pari_ulong));
			*primes = new_primes;
			size *= 2;
		}
	}
	// realloc to smaller size, to tightly fit all generated primes
	pari_ulong *new_primes =
	    pari_realloc(*primes, nprimes * sizeof(pari_ulong));
	if (new_primes) {
		*primes = new_primes;
	}
	avma = ltop;

	return nprimes;
}

static size_t invalid_curves(const curve_t *curve, const config_t *cfg,
                             pari_ulong *primes, size_t nprimes,
                             curve_t **curves, exhaustive_t *setup) {
	arg_t *invalid_argss[OFFSET_END];
	setup->argss = invalid_argss;

	// Alloc a curve, and only alloc a new one when this pointer is saved into
	// **curves
	curve_t *invalid = curve_new();
	// copy field + a from curve to invalid
	invalid->field = gcopy(curve->field);
	invalid->a = gcopy(curve->a);

	size_t ncurves = 0;
	while (ncurves < nprimes) {
		pari_sp btop = avma;
		// generate a curve with random b
		exhaustive_gen(invalid, cfg, setup, OFFSET_B, OFFSET_GENERATORS);

		// does some small prime from our array divide the curve order?
		// if so how many?
		size_t total = 0;
		for (size_t i = nprimes; i-- > 0;) {
			if (curves[i] == NULL && dvdis(invalid->order, primes[i])) {
				// whoo we have a new invalid curve
				if (!total && cfg->verbose) {
					fprintf(
					    verbose,
					    "we have a new one, calculating prime order points.\n");
				}
				total++;
			}
		}

		if (total > 0) {
			if (!exhaustive_gen_retry(invalid, cfg, setup, OFFSET_GENERATORS,
			                          OFFSET_POINTS, 1)) {
				curve_unroll(invalid, cfg, avma,
				             btop);  // necessary to free the ellinit
				avma = btop;
				continue;
			}

			// only pass small primes that divide the curve order and those
			// where we dont have a curve yet.
			// this is passed to points_gen_trial which uses trial division to
			// find
			// a point with given prime order.
			size_t j = 0;
			pari_ulong dprimes[total];
			for (size_t i = 0; i < nprimes; ++i) {
				if (curves[i] == NULL && dvdis(invalid->order, primes[i])) {
					if (cfg->verbose) {
						fprintf(verbose, "prime %lu divides curve order.\n",
						        primes[i]);
					}
					dprimes[j++] = primes[i];
				}
			}
			arg_t prime_divisors = {dprimes, total};
			invalid_argss[OFFSET_POINTS] = &prime_divisors;

			// generate prime order points, this is expensive (order needs to be
			// factorised, so only do it if we want the curve)
			exhaustive_gen(invalid, cfg, setup, OFFSET_POINTS, OFFSET_END);

			size_t count = 0;
			for (size_t i = nprimes; i-- > 0;) {
				if (curves[i] == NULL && dvdis(invalid->order, primes[i])) {
					if (count == 0) {
						// save a copy on first prime divisor from range
						curves[i] = invalid;
					} else {
						// copy if pointer already assigned
						curves[i] = curve_new_copy(invalid);
					}
					output_o(curves[i], cfg);
					if (ncurves != nprimes - 1) {
						output_o_separator(cfg);
					}
					ncurves++;
					count++;
				}
			}

			// copy the curve params that stay into a new curve, since this
			// curve pointer got assigned
			// this is for performance reasons. As it only copies the curve
			// and allocs memory if this curve is saved.
			invalid = curve_new();
			invalid->field = gcopy(curve->field);
			invalid->a = gcopy(curve->a);

			if (cfg->verbose) {
				fprintf(verbose,
				        "curve saved: %lu primes from range divide order.\n",
				        total);
				fprintf(verbose,
				        "curves done: %lu out of %lu needed. %.0f%% \n",
				        ncurves, nprimes, ((float)(ncurves) / nprimes) * 100);
			}
		} else {
			// here, we generated the curve but didn't use it, because no
			// primes from range divided order. Thus remove it
			// like it never existed.

			curve_unroll(invalid, cfg, avma,
			             btop);  // necessary to free the ellinit
			avma = btop;
		}
	}
	curve_free(&invalid);

	return ncurves;
}

static size_t invalid_curves_threaded(const curve_t *curve, const config_t *cfg,
                                      pari_ulong *primes, size_t nprimes,
                                      curve_t **curves, exhaustive_t *setup) {
	pthread_t pthreads[cfg->threads];
	thread_t threads[cfg->threads];
	struct pari_thread pari_threads[cfg->threads];
	pari_thread_sync();

	size_t generated = 0;
	state_e states[nprimes];
	state_e old_states[nprimes];
	curve_t *local_curves[nprimes];
	for (size_t i = 0; i < nprimes; ++i) {
		states[i] = STATE_FREE;
		old_states[i] = STATE_FREE;
	}
	pthread_mutex_t state_mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t generated_cond = PTHREAD_COND_INITIALIZER;

	for (size_t i = 0; i < cfg->threads; ++i) {
		pari_thread_alloc(&pari_threads[i], cfg->thread_memory,
		                  (GEN)&threads[i]);

		threads[i].pari_thread = &pari_threads[i];
		threads[i].original_curve = curve;
		threads[i].nprimes = nprimes;
		threads[i].primes = primes;
		threads[i].states = states;
		threads[i].curves = local_curves;
		threads[i].generated = &generated;
		threads[i].mutex_state = &state_mutex;
		threads[i].cond_generated = &generated_cond;
		threads[i].cfg = cfg;
		threads[i].setup = setup;
	}

	pthread_mutex_lock(&state_mutex);
	for (size_t i = 0; i < cfg->threads; ++i) {
		pthread_create(&pthreads[i], NULL, &invalid_thread,
		               (void *)&threads[i]);
	}

	bool running = true;
	do {
		pthread_cond_wait(&generated_cond, &state_mutex);
		debug("here %lu\n", generated);
		for (size_t i = 0; i < nprimes; ++i) {
			if (old_states[i] != states[i] && states[i] == STATE_GENERATED) {
				output_o(local_curves[i], cfg);
				if (generated != nprimes) {
					output_o_separator(cfg);
				}
				old_states[i] = states[i];
			}
		}

		if (generated == nprimes) running = false;
		pthread_mutex_unlock(&state_mutex);
	} while (running);

	for (size_t i = 0; i < cfg->threads; ++i) {
		pthread_join(pthreads[i], NULL);
	}

	for (size_t i = 0; i < nprimes; ++i) {
		curves[i] = curve_new_copy(local_curves[i]);
		curve_free(&local_curves[i]);
	}

	for (size_t i = 0; i < cfg->threads; ++i) {
		pari_thread_free(&pari_threads[i]);
	}
	pthread_mutex_destroy(&state_mutex);
	pthread_cond_destroy(&generated_cond);

	return generated;
}

int invalid_do(config_t *cfg) {
	debug_log_start("Starting Invalid curve method");

	gen_f original_gens[OFFSET_END] = {NULL};
	check_t *common_validators[OFFSET_END] = {NULL};
	arg_t *original_argss[OFFSET_END] = {NULL};
	unroll_f common_unrolls[OFFSET_END] = {NULL};
	invalid_original_ginit(original_gens, cfg);
	invalid_cinit(common_validators, cfg);
	exhaustive_uinit(common_unrolls, cfg);
	exhaustive_t original_setup = {.generators = original_gens,
	                               .validators = common_validators,
	                               .argss = original_argss,
	                               .unrolls = common_unrolls};

	debug_log_start("Starting to create curve to invalidate");
	curve_t *curve = curve_new();
	if (!exhaustive_gen(curve, cfg, &original_setup, OFFSET_FIELD,
	                    OFFSET_POINTS)) {
		curve_free(&curve);
		return EXIT_FAILURE;
	}
	debug_log_end("Finished creating curve to invalidate");

	output_o_begin(cfg);
	output_o(curve, cfg);
	output_o_separator(cfg);

	debug_log_start("Starting to generate primes to product over order^2");
	pari_ulong *primes;
	size_t nprimes = invalid_primes(curve->order, &primes);
	verbose_log("primes upto: p_max = %lu, n = %lu\n", primes[nprimes - 1],
	            nprimes);
	debug_log_end("Generated primes");

	curve_t **curves = try_calloc(nprimes * sizeof(curve_t *));

	// init the invalid curve gen_f
	gen_f invalid_gens[OFFSET_END];
	invalid_invalid_ginit(invalid_gens, cfg);
	exhaustive_t invalid_setup = {.generators = invalid_gens,
	                              .validators = common_validators,
	                              .argss = NULL,
	                              .unrolls = common_unrolls};

	debug_log_start("Starting to generate invalid curves");
	size_t ncurves;
	if (cfg->threads == 1) {
		ncurves =
		    invalid_curves(curve, cfg, primes, nprimes, curves, &invalid_setup);
	} else {
		ncurves = invalid_curves_threaded(curve, cfg, primes, nprimes, curves,
		                                  &invalid_setup);
	}
	debug_log_end("Finished generating invalid curves");
	output_o_end(cfg);

	for (size_t i = 0; i < ncurves; ++i) {
		curve_free(&curves[i]);
	}
	try_free(curves);
	try_free(primes);
	curve_free(&curve);

	debug_log_end("Finished Invalid curve method");
	return EXIT_SUCCESS;
}
