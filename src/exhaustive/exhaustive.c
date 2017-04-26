/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "exhaustive.h"
#include <io/config.h>
#include "io/output.h"
#include "math/arg.h"
#include "math/curve.h"
#include "math/equation.h"
#include "math/field.h"
#include "math/gens.h"
#include "math/order.h"
#include "math/point.h"
#include "seed.h"

static void exhaustive_ginit(gen_t *generators, const config_t *cfg) {
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
		case POINTS_NONE:
			generators[OFFSET_POINTS] = &gen_skip;
			break;
	}
}

static void exhaustive_ainit(arg_t **argss, const config_t *cfg) {
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

void exhaustive_uinit(unroll_t *unrolls, const config_t *cfg) {
	unrolls[OFFSET_FIELD] = &unroll_skip;
	unrolls[OFFSET_A] = &unroll_skip;
	unrolls[OFFSET_B] = &unroll_skip;
	unrolls[OFFSET_CURVE] = &curve_unroll;
	unrolls[OFFSET_ORDER] = &unroll_skip;
	unrolls[OFFSET_GENERATORS] = &gens_unroll;
	unrolls[OFFSET_POINTS] = &points_unroll;
}

int exhaustive_gen_retry(curve_t *curve, const config_t *cfg,
                         gen_t generators[], arg_t *argss[], unroll_t unrolls[],
                         offset_e start_offset, offset_e end_offset,
                         int retry) {
	if (start_offset == end_offset) {
		return 1;
	}
	if (start_offset > end_offset) {
		return -1;
	}
	int num_gens = end_offset - start_offset;
	pari_sp tops[num_gens];
	int tries[num_gens];
	memset(tries, 0, sizeof(int) * num_gens);

	int state = start_offset;
	while (state < end_offset) {
		// remember pari stack
		tops[state - start_offset] = avma;

		int diff = generators[state](curve, cfg, argss ? argss[state] : NULL);
		if (diff == INT_MIN) {
			fprintf(stderr, "Error generating a curve. state = %i\n", state);
			return 0;
		}

		if (state + diff < start_offset) {
			// what now?
			// TODO
		} else if (diff <= 0) {
			// unroll pari stack
			int new_state = state + diff;
			for (int i = state; i > new_state;) {
				if (unrolls && unrolls[i]) {
					i += unrolls[i](curve, cfg, tops[i], tops[i - 1]);
				} else {
					--i;
				}
			}
			avma = tops[new_state - start_offset];
		}

		if (diff == 0) {
			int tried = ++tries[state - start_offset];
			if (retry && tried >= retry) {
				fprintf(stderr, "Reached retry limit: %i, state = %i\n", retry,
				        state);
				return 0;
			}
		} else if (diff > 0) {
			tries[state - start_offset] = 0;
		}
		state += diff;

		if (cfg->verbose) {
			if (diff > 0) {
				fprintf(verbose, "+");
			} else if (diff < 0) {
				fprintf(verbose, "-");
			} else {
				fprintf(verbose, ".");
			}
		}
	}

	if (cfg->verbose) fprintf(verbose, "\n");

	return 1;
}

int exhaustive_gen(curve_t *curve, const config_t *cfg, gen_t generators[],
                   arg_t *argss[], unroll_t unrolls[], offset_e start_offset,
                   offset_e end_offset) {
	return exhaustive_gen_retry(curve, cfg, generators, argss, unrolls,
	                            start_offset, end_offset, 0);
}

static void exhaustive_quit(arg_t *argss[]) {
	equation_quit();
	for (size_t i = 0; i < OFFSET_END; ++i) {
		if (argss[i]) {
			arg_free(&(argss[i]));
		}
	}
}

int exhaustive_do(config_t *cfg) {
	debug("# Starting Exhaustive method\n");

	gen_t generators[OFFSET_END];
	arg_t *argss[OFFSET_END];
	unroll_t unrolls[OFFSET_END];
	exhaustive_ginit(generators, cfg);
	exhaustive_ainit(argss, cfg);
	exhaustive_uinit(unrolls, cfg);

	output_o_begin(cfg);
	for (unsigned long i = 0; i < cfg->count; ++i) {
		curve_t *curve = curve_new();
		if (!exhaustive_gen_retry(curve, cfg, generators, argss, unrolls,
		                          OFFSET_SEED, OFFSET_END, 10)) {
			curve_free(&curve);
			return 1;
		}
		output_o(curve, cfg);
		if (i != cfg->count - 1) {
			output_o_separator(cfg);
		}
		curve_free(&curve);
	}
	output_o_end(cfg);

	exhaustive_quit(argss);
	return 0;
}
