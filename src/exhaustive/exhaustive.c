/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "exhaustive.h"
#include "io/output.h"
#include "math/arg.h"
#include "math/curve.h"
#include "math/equation.h"
#include "math/field.h"
#include "math/gens.h"
#include "math/order.h"
#include "math/point.h"
#include "seed.h"

void exhaustive_ginit(gen_t *generators, config_t *cfg) {
	if (cfg->from_seed) {
		if (cfg->seed) {
			generators[OFFSET_SEED] = &seed_argument;
		} else {
			if (cfg->random) {
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

		if (cfg->random) {
			generators[OFFSET_A] = &a_random;
			generators[OFFSET_B] = &b_random;
		} else {
			generators[OFFSET_A] = &a_input;
			generators[OFFSET_B] = &b_input;
		}

		if (cfg->koblitz) {
			generators[OFFSET_A] = &a_zero;
		}

		generators[OFFSET_CURVE] = &curve_nonzero;

		if (cfg->prime) {
			generators[OFFSET_ORDER] = &order_prime;
		} else if (cfg->cofactor) {
			generators[OFFSET_ORDER] = &order_smallfact;
		} else {
			generators[OFFSET_ORDER] = &order_any;
		}
	}
	if (cfg->unique) {
		generators[OFFSET_GENERATORS] = &gens_one;
	} else {
		generators[OFFSET_GENERATORS] = &gens_any;
	}

	if (cfg->random) {
		generators[OFFSET_FIELD] = &field_random;
	} else {
		generators[OFFSET_FIELD] = &field_input;
	}

	switch (cfg->points.type) {
		case POINTS_RANDOM:
			if (cfg->points.amount) {
				generators[OFFSET_POINTS] = &points_random;
			} else {
				generators[OFFSET_POINTS] = &point_random;
			}
			break;
		case POINTS_PRIME:
			generators[OFFSET_POINTS] = &points_prime;
			break;
	}
}

void exhaustive_ainit(arg_t **argss, config_t *cfg) {
	for (size_t i = 0; i < OFFSET_END; ++i) {
		argss[i] = NULL;
	}
	if (cfg->points.type == POINTS_RANDOM) {
		arg_t *points_arg = arg_new();
		points_arg->args = &cfg->points.amount;
		points_arg->nargs = 1;
		argss[OFFSET_POINTS] = points_arg;
	}
	if (cfg->cofactor) {
		arg_t *order_arg = arg_new();
		order_arg->args = &cfg->cofactor_bound;
		order_arg->nargs = 1;
		argss[OFFSET_ORDER] = order_arg;
	}
}

int exhaustive_gen(curve_t *curve, config_t *cfg, gen_t generators[],
                   arg_t *argss[], int start_offset, int end_offset) {
	if (start_offset == end_offset) {
		return 1;
	}
	if (start_offset > end_offset) {
		return 0;
	}
	pari_sp tops[end_offset - start_offset];

	int state = start_offset;
	while (state < end_offset) {
		tops[state - start_offset] = avma;

		int diff = generators[state](curve, cfg, argss ? argss[state] : NULL);
		if (diff == INT_MIN) {
			fprintf(stderr, "Error generating a curve. %i\n", state);
			return 0;
		}
		state += diff;

		if (diff <= 0) {
			avma = tops[state - start_offset];
		}

		if (cfg->verbose) {
			if (diff > 0) {
				fprintf(debug, "+");
			} else if (diff < 0) {
				fprintf(debug, "-");
			} else {
				fprintf(debug, ".");
			}
			fflush(debug);
		}
	}

	if (cfg->verbose) fprintf(debug, "\n");

	return 1;
}

void exhaustive_quit(arg_t *argss[]) {
	equation_quit();
	for (size_t i = 0; i < OFFSET_END; ++i) {
		if (argss[i]) {
			arg_free(&(argss[i]));
		}
	}
}

int exhaustive_do(config_t *cfg) {
	gen_t generators[OFFSET_END];
	arg_t *argss[OFFSET_END];
	exhaustive_ginit(generators, cfg);
	exhaustive_ainit(argss, cfg);

	for (long i = 0; i < cfg->count; ++i) {
		curve_t *curve = curve_new();
		if (!exhaustive_gen(curve, cfg, generators, argss, OFFSET_SEED,
		                    OFFSET_END)) {
			curve_free(&curve);
			return 1;
		}
		output_o(curve, cfg);
		curve_free(&curve);
	}

	exhaustive_quit(argss);
	return 0;
}
