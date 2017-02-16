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
#include "math/point.h"
#include "seed.h"

void exhaustive_ginit(gen_t *generators, config_t *config) {
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

	generators[OFFSET_POINTS] = &points_prime;
}

void exhaustive_vinit(arg_t *argss[], config_t *config) {
	// TODO implement when for example points_random is used...
}

int exhaustive_gen(curve_t *curve, config_t *config, gen_t generators[],
                   arg_t *argss[], int start_offset, int end_offset) {
	int state = start_offset;
	while (state != end_offset) {
		int diff =
		    generators[state](curve, config, argss ? argss[state] : NULL);
		if (diff == INT_MIN) {
			fprintf(stderr, "Error generating a curve. %i\n", state);
			return 0;
		}
		if (config->verbose) {
			if (diff > 0) {
				fprintf(debug, "+");
			} else if (diff < 0) {
				fprintf(debug, "-");
			} else {
				fprintf(debug, ".");
			}
			fflush(debug);
		}
		state += diff;
	}

	if (config->verbose) fprintf(debug, "\n");

	return 1;
}

int exhaustive_do(config_t *cfg) {
	gen_t generators[OFFSET_END];
	arg_t *argss[OFFSET_END];
	exhaustive_ginit(generators, cfg);
	exhaustive_vinit(argss, cfg);

	curve_t *curve = curve_new();
	if (!exhaustive_gen(curve, cfg, generators, argss, OFFSET_SEED,
	                    OFFSET_END)) {
		curve_free(&curve);
		return 1;
	}
	output_o(curve, cfg);
	curve_free(&curve);
	return 0;
}
