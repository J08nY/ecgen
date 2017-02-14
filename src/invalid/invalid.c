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
#include "math/order.h"
#include "math/point.h"

void invalid_init(gen_t generators[], config_t *cfg) {
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
	// Have primes, now generate random b
	gen_t invalid_gen[OFFSET_END];
	invalid_gen[OFFSET_FIELD] = &gen_skip;
	invalid_gen[OFFSET_A] = &gen_skip;
	invalid_gen[OFFSET_B] = &b_random;
	invalid_gen[OFFSET_CURVE] = &curve_nonzero;
	invalid_gen[OFFSET_ORDER] = &order_init;
	invalid_gen[OFFSET_POINTS] = &points_prime;

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
		exhaustive_gen(invalid, cfg, invalid_gen, OFFSET_B, OFFSET_END);

		// does some small prime from our array divide the curve order?
		size_t count = 0;
		for (size_t i = nprimes; i-- > 0;) {
			if (dvdis(invalid->order, primes[i]) && (*curves)[i] == NULL) {
				if (count == 0) {
					(*curves)[i] = invalid;
				} else {
					(*curves)[i] = curve_new();
					(*curves)[i] = curve_copy(invalid, (*curves)[i]);
				}
				output_csv(out, "%P#x", ',', curve_params((*curves)[i]));
				ncurves++;
				count++;
			}
		}
		if (count > 0) {
			invalid = curve_new();
			invalid->field = gcopy(curve->field);
			invalid->a = gcopy(curve->a);
		} else {
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
	invalid_init(gen, cfg);

	// actually generate the curve
	if (!exhaustive_gen(curve, cfg, gen, OFFSET_FIELD, OFFSET_POINTS)) {
		curve_free(&curve);
		return 1;
	}
	output_csv(out, "%P#x", ',', curve_params(curve));

	// now, generate primes upto order^2
	pari_ulong *primes;
	size_t nprimes = invalid_primes(curve->order, &primes);

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