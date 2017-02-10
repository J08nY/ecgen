/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "exhaustive.h"
#include "io/output.h"
#include "math/curve.h"
#include "math/equation.h"
#include "math/field.h"
#include "math/order.h"
#include "seed.h"

void exhaustive_init(gen_t generators[], config_t *config) {
	if (config->from_seed) {
		if (config->seed) {
			generators[OFFSET_SEED] = &seed_argument;
		} else {
			if (config->random) {
				generators[OFFSET_SEED] = &seed_random;
			} else {
				generators[OFFSET_SEED] = &seed_input;
			}
		}
		generators[OFFSET_A] = &a_seed;
		generators[OFFSET_B] = &b_seed;
		generators[OFFSET_CURVE] = &curve_seed;
	} else {
		generators[OFFSET_SEED] = &gen_skip;

		if (config->random) {
			generators[OFFSET_A] = &a_random;
			generators[OFFSET_B] = &b_random;
		} else {
			generators[OFFSET_A] = &a_input;
			generators[OFFSET_B] = &b_input;
		}

		if (config->koblitz) {
			generators[OFFSET_A] = &a_zero;
		}

		generators[OFFSET_CURVE] = &curve_nonzero;

		if (config->prime) {
			generators[OFFSET_ORDER] = &order_prime;
		} else {
			generators[OFFSET_ORDER] = &order_init;
		}
	}

	if (config->random) {
		generators[OFFSET_FIELD] = &field_random;
	} else {
		generators[OFFSET_FIELD] = &field_input;
	}
}

int exhaustive_do(config_t *cfg) {
	gen_t generators[OFFSET_END];
	exhaustive_init(generators, cfg);

	curve_t *curve = curve_new();
	int state = 0;
	while (state != OFFSET_POINTS) {
		int diff = generators[state](curve, cfg);
		state += diff;
	}
	output_csv(out, "%Px", ';', curve_params(curve));
	curve_free(&curve);
	return 0;
}