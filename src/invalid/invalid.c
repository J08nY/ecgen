/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "invalid.h"
#include "exhaustive/arg.h"
#include "exhaustive/check.h"
#include "exhaustive/exhaustive.h"
#include "gen/curve.h"
#include "gen/equation.h"
#include "gen/field.h"
#include "gen/gens.h"
#include "gen/order.h"
#include "gen/point.h"
#include "invalid_thread.h"
#include "obj/curve.h"
#include "util/memory.h"

static void invalid_original_ginit(gen_f *generators) {
	generators[OFFSET_SEED] = &gen_skip;
	if (cfg->random & RANDOM_FIELD) {
		generators[OFFSET_FIELD] = &field_gen_random;
	} else {
		generators[OFFSET_FIELD] = &field_gen_input;
	}
	if (cfg->random & RANDOM_A) {
		generators[OFFSET_A] = &a_gen_random;
	} else {
		generators[OFFSET_A] = &a_gen_input;
	}
	if (cfg->random & RANDOM_B) {
		generators[OFFSET_B] = &b_gen_random;
	} else {
		generators[OFFSET_B] = &b_gen_input;
	}
	generators[OFFSET_GENERATORS] = &gens_gen_any;
	generators[OFFSET_CURVE] = &curve_gen_any;
	generators[OFFSET_ORDER] = &order_gen_any;
	generators[OFFSET_METADATA] = &gen_skip;
}

static void invalid_invalid_ginit(gen_f *generators) {
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
	generators[OFFSET_METADATA] = &gen_skip;
}

static void invalid_cinit(check_t **validators) {
	check_t *curve_check = check_new(curve_check_nonzero, NULL);
	validators[OFFSET_CURVE] = curve_check;
}

static void invalid_init(exhaustive_t *setup) {
	invalid_cinit(setup->validators);
	exhaustive_uinit(setup->unrolls);
}

static size_t invalid_primes(GEN order, pari_ulong **primes) {
	pari_sp ltop = avma;

	GEN bound = sqri(order);
	GEN product = gen_1;
	pari_ulong last = 1;
	pari_ulong upper = 0;
	size_t nprimes = 0;

	if (cfg->invalid_primes) {
		char *end = NULL;
		last = (pari_ulong)strtol(cfg->invalid_primes, &end, 10) - 1;
		if (end && *end) {
			end++;
			upper = (pari_ulong)strtol(end, NULL, 10);
		} else {
			upper = unextprime(last + 1);
		}
	}

	size_t size = 10;
	*primes = try_calloc(size * sizeof(pari_ulong));

	while (((upper == 0) && cmpii(bound, product) >= 0) || (last <= upper)) {
		product = mulis(product, last);
		pari_ulong next = unextprime(last + 1);
		if ((upper != 0) && next > upper) {
			break;
		}
		(*primes)[nprimes] = next;
		last = (*primes)[nprimes];
		nprimes++;
		if (nprimes == size) {
			pari_ulong *new_primes =
			    try_realloc(*primes, size * 2 * sizeof(pari_ulong));
			*primes = new_primes;
			size *= 2;
		}
	}
	/* realloc to smaller size, to tightly fit all generated primes */
	pari_ulong *new_primes =
	    pari_realloc(*primes, nprimes * sizeof(pari_ulong));
	if (new_primes) {
		*primes = new_primes;
	}
	avma = ltop;

	return nprimes;
}

static size_t invalid_curves_single(const curve_t *curve, pari_ulong *primes,
                                    size_t nprimes, curve_t **curves,
                                    exhaustive_t *setup) {
	arg_t *invalid_argss[OFFSET_END] = {NULL};
	exhaustive_t invalid_setup = {.generators = setup->generators,
	                              .gen_argss = invalid_argss,
	                              .validators = setup->validators,
	                              .check_argss = setup->check_argss,
	                              .unrolls = setup->unrolls};

	curve_t *invalid = curve_new();
	/* copy field + a from curve to invalid */
	invalid->field = gcopy(curve->field);
	invalid->a = gcopy(curve->a);

	size_t ncurves = 0;
	while (ncurves < nprimes) {
		pari_sp btop = avma;
		/* generate a curve with random b */
		exhaustive_gen(invalid, setup, OFFSET_B, OFFSET_GENERATORS);

		/*
		 * does some small prime from our array divide the curve order?
		 * if so how many?
		 */
		size_t total = 0;
		for (size_t i = nprimes; i-- > 0;) {
			if (curves[i] == NULL && dvdis(invalid->order, primes[i])) {
				if (!total) {
					verbose_log(
					    "we have a new one, calculating prime order points.\n");
				}
				total++;
			}
		}

		if (total > 0) {
			if (!exhaustive_gen_retry(invalid, setup, OFFSET_GENERATORS,
			                          OFFSET_POINTS, 1)) {
				curve_unroll(invalid, avma,
				             btop);  // necessary to free the ellinit
				avma = btop;
				continue;
			}

			/*
			 * only pass small primes that divide the curve order and those
			 * where we dont have a curve yet.
			 * this is passed to points_gen_trial which uses trial division to
			 * find
			 * a point with given prime order.
			 */
			size_t j = 0;
			pari_ulong dprimes[total];
			for (size_t i = 0; i < nprimes; ++i) {
				if (curves[i] == NULL && dvdis(invalid->order, primes[i])) {
					verbose_log("prime %lu divides curve order.\n", primes[i]);
					dprimes[j++] = primes[i];
				}
			}
			arg_t prime_divisors = {dprimes, total};
			invalid_argss[OFFSET_POINTS] = &prime_divisors;

			/*
			 * generate prime order points, this is expensive (order needs to be
			 * factorised, so only do it if we want the curve)
			 */
			exhaustive_gen(invalid, &invalid_setup, OFFSET_POINTS, OFFSET_END);

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
					output_o(curves[i]);
					if (ncurves != nprimes - 1) {
						output_o_separator();
					}
					ncurves++;
					count++;
				}
			}

			/*
			 * copy the curve params that stay into a new curve, since this
			 * curve pointer got assigned
			 * this is for performance reasons. As it only copies the curve
			 * and allocs memory if this curve is saved.
			 */
			invalid = curve_new();
			invalid->field = gcopy(curve->field);
			invalid->a = gcopy(curve->a);

			verbose_log("curve saved: %lu primes from range divide order.\n",
			            total);
			verbose_log("curves done: %lu out of %lu needed. %.0f%% \n",
			            ncurves, nprimes, ((float)(ncurves) / nprimes) * 100);
		} else {
			/*
			 * here, we generated the curve but didn't use it, because no
			 * primes from range divided order. Thus unroll it
			 * like it never existed, but don't free it yet.
			 */

			curve_unroll(invalid, avma, btop);
			avma = btop;
		}
	}
	curve_free(&invalid);

	return ncurves;
}

static size_t invalid_curves_threaded(const curve_t *curve, pari_ulong *primes,
                                      size_t nprimes, curve_t **curves,
                                      exhaustive_t *setup) {
	pthread_t pthreads[cfg->threads];
	thread_t threads[cfg->threads];
	struct pari_thread pari_threads[cfg->threads];

	size_t *generated = try_calloc(sizeof(size_t));
	state_e *states = try_calloc(sizeof(state_e) * nprimes);
	state_e *old_states = try_calloc(sizeof(state_e) * nprimes);
	curve_t **local_curves = try_calloc(sizeof(curve_t *) * nprimes);
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
		threads[i].generated = generated;
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

	while (true) {
		pthread_cond_wait(&generated_cond, &state_mutex);
		for (size_t i = 0; i < nprimes; ++i) {
			if (old_states[i] != states[i] && states[i] == STATE_GENERATED) {
				output_o(local_curves[i]);
				if (*generated != nprimes) {
					output_o_separator();
				}
				old_states[i] = states[i];
			}
		}

		if (*generated == nprimes)
			break;
	}
	pthread_mutex_unlock(&state_mutex);

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

	size_t result = *generated;
	try_free(generated);
	try_free(states);
	try_free(old_states);
	try_free(local_curves);

	return result;
}

curve_t *invalid_original_curve(exhaustive_t *setup) {
	curve_t *curve = curve_new();
	if (!exhaustive_gen(curve, setup, OFFSET_FIELD, OFFSET_POINTS)) {
		exhaustive_clear(setup);
		curve_free(&curve);
		exit(EXIT_FAILURE);
	}
	return curve;
}

int invalid_do() {
	debug_log_start("Starting Invalid curve method");

	gen_f original_gens[OFFSET_END] = {NULL};
	gen_f invalid_gens[OFFSET_END] = {NULL};
	check_t *common_validators[OFFSET_END] = {NULL};
	arg_t *common_gen_argss[OFFSET_END] = {NULL};
	arg_t *common_check_argss[OFFSET_END] = {NULL};
	unroll_f common_unrolls[OFFSET_END] = {NULL};

	exhaustive_t original_setup = {.generators = original_gens,
	                               .gen_argss = common_gen_argss,
	                               .validators = common_validators,
	                               .check_argss = common_check_argss,
	                               .unrolls = common_unrolls};
	invalid_init(&original_setup);
	invalid_original_ginit(original_gens);

	exhaustive_t invalid_setup = {.generators = invalid_gens,
	                              .gen_argss = common_gen_argss,
	                              .validators = common_validators,
	                              .check_argss = common_check_argss,
	                              .unrolls = common_unrolls};
	invalid_invalid_ginit(invalid_gens);

	debug_log_start("Starting to create curve to invalidate");
	curve_t *curve = invalid_original_curve(&original_setup);
	debug_log_end("Finished creating curve to invalidate");

	output_o_begin();
	output_o(curve);
	output_o_separator();

	debug_log_start("Starting to generate primes to product over order^2");
	pari_ulong *primes;
	size_t nprimes = invalid_primes(curve->order, &primes);
	debug_log_end("Generated primes");

	if (!nprimes) {
		fprintf(err, "No orders to generate curves for, quitting.");
		return EXIT_FAILURE;
	}

	curve_t **curves = try_calloc(nprimes * sizeof(curve_t *));
	debug_log_start("Starting to generate invalid curves");
	size_t ncurves;
	if (cfg->threads == 1) {
		ncurves = invalid_curves_single(curve, primes, nprimes, curves,
		                                &invalid_setup);
	} else {
		ncurves = invalid_curves_threaded(curve, primes, nprimes, curves,
		                                  &invalid_setup);
	}
	debug_log_end("Finished generating invalid curves");
	output_o_end();

	for (size_t i = 0; i < ncurves; ++i) {
		curve_free(&curves[i]);
	}
	try_free(curves);
	try_free(primes);
	curve_free(&curve);
	exhaustive_clear(&original_setup);
	exhaustive_clear(&invalid_setup);

	debug_log_end("Finished Invalid curve method");
	return EXIT_SUCCESS;
}
