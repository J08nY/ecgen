/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "invalid.h"
#include "exhaustive/exhaustive.h"
#include "io/output.h"
#include "math/curve.h"
#include "math/equation.h"
#include "math/field.h"
#include "math/gens.h"
#include "math/order.h"
#include "math/point.h"

void invalid_ginit(gen_t *generators, config_t *cfg) {
	generators[OFFSET_SEED] = &gen_skip;
	if (cfg->random) {
		generators[OFFSET_FIELD] = &field_random;
		generators[OFFSET_A] = &a_random;
		generators[OFFSET_B] = &b_random;
	} else {
		generators[OFFSET_FIELD] = &field_input;
		generators[OFFSET_A] = &a_input;
		generators[OFFSET_B] = &b_input;
	}
	generators[OFFSET_CURVE] = &curve_nonzero;
	generators[OFFSET_ORDER] = &order_init;
}

size_t invalid_primes(GEN order, pari_ulong **primes) {
	pari_sp ltop = avma;

	GEN bound = sqri(order);
	GEN product = gen_1;
	pari_ulong last = 1;
	size_t nprimes = 0;

	size_t size = 10;
	*primes = pari_malloc(size * sizeof(pari_ulong));
	while (cmpii(bound, product) >= 0) {
		product = mulis(product, last);
		(*primes)[nprimes] = unextprime(last + 1);
		last = (*primes)[nprimes];
		nprimes++;
		if (nprimes == size) {
			pari_ulong *new_primes =
			    pari_realloc(*primes, size * 2 * sizeof(pari_ulong));
			if (new_primes) {
				*primes = new_primes;
				size *= 2;
			} else {
				perror("Couldn't malloc.");
				return 0;
			}
		}
	}
	// realloc to smaller size, to tightly fit all generated primes
	pari_ulong *new_primes =
	    pari_realloc(*primes, nprimes * sizeof(pari_ulong));
	if (new_primes) {
		*primes = new_primes;
	} else {
		perror("Couldn't malloc.");
		return 0;
	}

	avma = ltop;

	return nprimes;
}

size_t invalid_curves(curve_t *curve, config_t *cfg, pari_ulong *primes,
                      size_t nprimes, curve_t ***curves) {
	gen_t invalid_gen[OFFSET_END];
	invalid_gen[OFFSET_FIELD] = &gen_skip;
	invalid_gen[OFFSET_A] = &gen_skip;
	invalid_gen[OFFSET_B] = &b_random;
	invalid_gen[OFFSET_CURVE] = &curve_nonzero;
	invalid_gen[OFFSET_ORDER] = &order_init;
	invalid_gen[OFFSET_GENERATORS] = &gens_init;
	invalid_gen[OFFSET_POINTS] = &points_primet;

	arg_t *invalid_argss[OFFSET_END];

	// We will have nprimes curves in the end
	*curves = pari_malloc(nprimes * sizeof(curve_t *));
	if (!(*curves)) {
		perror("Couldn't malloc.");
		return 0;
	}
	memset(*curves, 0, nprimes * sizeof(curve_t *));

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
		exhaustive_gen(invalid, cfg, invalid_gen, NULL, OFFSET_B,
		               OFFSET_GENERATORS);

		// does some small prime from our array divide the curve order?
		// if so how many?
		size_t total = 0;
		for (size_t i = nprimes; i-- > 0;) {
			if ((*curves)[i] == NULL && dvdis(invalid->order, primes[i])) {
				// whoo we have a new invalid curve
				if (!total && cfg->verbose) {
					fprintf(
					    debug,
					    "we have a new one, calculating prime order points.\n");
				}
				total++;
			}
		}

		if (total > 0) {
			// only pass small primes that divide the curve order and those
			// where we dont have a curve yet.
			// this is passed to points_primet which uses trial division to find
			// a point with given prime order.
			size_t j = 0;
			pari_ulong dprimes[total];
			for (size_t i = 0; i < nprimes; ++i) {
				if ((*curves)[i] == NULL && dvdis(invalid->order, primes[i])) {
					if (cfg->verbose) {
						fprintf(debug, "prime %lu divides curve order.\n",
						        primes[i]);
					}
					dprimes[j++] = primes[i];
				}
			}
			arg_t prime_divisors = {dprimes, total};
			invalid_argss[OFFSET_POINTS] = &prime_divisors;

			// generate prime order points, this is expensive (order needs to be
			// factorised, so only do it if we want the curve)
			exhaustive_gen(invalid, cfg, invalid_gen, invalid_argss,
			               OFFSET_GENERATORS, OFFSET_END);

			size_t count = 0;
			for (size_t i = nprimes; i-- > 0;) {
				if ((*curves)[i] == NULL && dvdis(invalid->order, primes[i])) {
					if (count == 0) {
						// save a copy on first prime divisor from range
						(*curves)[i] = invalid;
					} else {
						// copy if pointer already assigned
						(*curves)[i] = curve_new();
						(*curves)[i] = curve_copy(invalid, (*curves)[i]);
					}
					output_o((*curves)[i], cfg);
					ncurves++;
					count++;
				}
			}

			// copy the curve params that stay into a new curve, since this
			// curve pointer got assigned
			// this is for performance reasons. As it only copies the curve
			// and mallocs memory if this curve is saved.
			invalid = curve_new();
			invalid->field = gcopy(curve->field);
			invalid->a = gcopy(curve->a);

			if (cfg->verbose) {
				fprintf(debug,
				        "curve saved: %lu primes from range divide order.\n",
				        total);
				fprintf(debug, "curves done: %lu out of %lu needed. %.0f%% \n",
				        ncurves, nprimes, ((float)(ncurves) / nprimes) * 100);
			}
		} else {
			// here, we generated the curve but didn't use it, because no
			// primes from range divided order. Thus remove it
			// like it never existed.

			points_free_deep(&invalid->points, invalid->npoints);
			avma = btop;
		}
	}
	return ncurves;
}

int invalid_do(config_t *cfg) {
	// create the curve to invalidate
	// Either from input or random with -r
	curve_t *curve = curve_new();
	gen_t gen[OFFSET_END];
	arg_t *argss[OFFSET_END];
	invalid_ginit(gen, cfg);

	// actually generate the curve
	if (!exhaustive_gen(curve, cfg, gen, argss, OFFSET_FIELD, OFFSET_POINTS)) {
		curve_free(&curve);
		return 1;
	}
	output_o(curve, cfg);

	// now, generate primes upto order^2
	pari_ulong *primes;
	size_t nprimes = invalid_primes(curve->order, &primes);

	if (cfg->verbose) {
		fprintf(debug, "primes upto: p_max = %lu, n = %lu\n",
		        primes[nprimes - 1], nprimes);
	}

	curve_t **curves;
	size_t ncurves = invalid_curves(curve, cfg, primes, nprimes, &curves);

	for (size_t i = 0; i < ncurves; ++i) {
		curve_free(&curves[i]);
	}
	pari_free(curves);
	pari_free(primes);
	curve_free(&curve);

	return 0;
}
