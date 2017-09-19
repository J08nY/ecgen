/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "exhaustive.h"
#include <misc/config.h>
#include "anomalous.h"
#include "ansi.h"
#include "gen/curve.h"
#include "gen/equation.h"
#include "gen/field.h"
#include "gen/gens.h"
#include "gen/order.h"
#include "gen/point.h"
#include "io/output.h"
#include "util/memory.h"

static void exhaustive_ginit(gen_t *generators, const config_t *cfg) {
	if (cfg->ansi) {
		// setup ANSI X9.62 generators
		if (cfg->seed) {
			generators[OFFSET_SEED] = &ansi_gen_seed_argument;
		} else {
			if (cfg->random) {
				generators[OFFSET_SEED] = &ansi_gen_seed_random;
			} else {
				generators[OFFSET_SEED] = &ansi_gen_seed_input;
			}
		}
		generators[OFFSET_A] = &gen_skip;
		generators[OFFSET_B] = &ansi_gen_equation;
		generators[OFFSET_CURVE] = &curve_gen_nonzero;
		generators[OFFSET_ORDER] = &order_gen_any;
	} else {
		// setup normal generators
		generators[OFFSET_SEED] = &gen_skip;

		if (cfg->anomalous) {
			generators[OFFSET_A] = &gen_skip;
			generators[OFFSET_B] = &anomalous_gen_equation;
		} else if (cfg->random) {
			generators[OFFSET_A] = &a_gen_random;
			generators[OFFSET_B] = &b_gen_random;
		} else {
			generators[OFFSET_A] = &a_gen_input;
			generators[OFFSET_B] = &b_gen_input;
		}

		if (cfg->koblitz) {
			switch (cfg->koblitz_value) {
				case 0:
					generators[OFFSET_A] = &a_gen_zero;
					break;
				case 1:
					generators[OFFSET_A] = &a_gen_one;
					break;
				default:
					break;
			}
			generators[OFFSET_B] = &b_gen_one;
		}

		generators[OFFSET_CURVE] = &curve_gen_nonzero;

		if (cfg->prime) {
			generators[OFFSET_ORDER] = &order_gen_prime;
		} else if (cfg->cofactor) {
			generators[OFFSET_ORDER] = &order_gen_smallfact;
		} else if (cfg->anomalous) {
			generators[OFFSET_ORDER] = &anomalous_gen_order;
		} else {
			generators[OFFSET_ORDER] = &order_gen_any;
		}
	}

	// setup common generators
	if (cfg->unique) {
		generators[OFFSET_GENERATORS] = &gens_gen_one;
	} else {
		generators[OFFSET_GENERATORS] = &gens_gen_any;
	}

	if (cfg->anomalous) {
		generators[OFFSET_FIELD] = &anomalous_gen_field;
	} else if (cfg->random) {
		generators[OFFSET_FIELD] = &field_gen_random;
	} else {
		generators[OFFSET_FIELD] = &field_gen_input;
	}

	switch (cfg->points.type) {
		case POINTS_RANDOM:
			if (cfg->points.amount) {
				generators[OFFSET_POINTS] = &points_gen_random;
			} else {
				generators[OFFSET_POINTS] = &point_gen_random;
			}
			break;
		case POINTS_PRIME:
			generators[OFFSET_POINTS] = &points_gen_prime;
			break;
		case POINTS_NONPRIME:
			generators[OFFSET_POINTS] = &points_gen_nonprime;
			break;
		case POINTS_ALL:
			generators[OFFSET_POINTS] = &points_gen_allgroups;
			break;
		case POINTS_NONE:
			generators[OFFSET_POINTS] = &gen_skip;
			break;
	}
}

static void exhaustive_ainit(arg_t **argss, const config_t *cfg) {
	if (cfg->anomalous) {
		arg_t *field_arg = arg_new();
		arg_t *eq_arg = arg_new();
		size_t *i = try_calloc(sizeof(size_t));
		*i = 3;
		field_arg->args = i;
		field_arg->nargs = 1;
		eq_arg->args = i;
		eq_arg->nargs = 1;
		eq_arg->allocd = i;
		argss[OFFSET_FIELD] = field_arg;
		argss[OFFSET_B] = eq_arg;
	}
	if (cfg->points.type == POINTS_RANDOM) {
		arg_t *points_arg = arg_new();
		points_arg->args = &cfg->points.amount;
		points_arg->nargs = 1;
		argss[OFFSET_POINTS] = points_arg;
	}
	if (cfg->cofactor) {
		arg_t *order_arg = arg_new();
		arg_t *gens_arg = arg_new();
		order_arg->args = &cfg->cofactor_bound;
		order_arg->nargs = 1;
		gens_arg->args = &cfg->cofactor_bound;
		gens_arg->nargs = 1;
		argss[OFFSET_ORDER] = order_arg;
		argss[OFFSET_GENERATORS] = gens_arg;
	}
}

void exhaustive_uinit(unroll_t *unrolls, const config_t *cfg) {
	if (cfg->ansi) {
		unrolls[OFFSET_SEED] = &ansi_unroll_seed;
	} else {
		unrolls[OFFSET_SEED] = &unroll_skip;
	}
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
		return 2;
	}
	if (start_offset > end_offset) {
		return 0;
	}

	pari_sp stack_tops[OFFSET_END] = {0};
	int gen_tries[OFFSET_END] = {0};

	int state = start_offset;
	while (state < end_offset) {
		stack_tops[state] = avma;

		int diff = generators[state](curve, cfg, argss ? argss[state] : NULL);
		int new_state = state + diff;
		if (new_state < start_offset) new_state = start_offset;

		if (diff <= 0) {
			if (diff == INT_MIN || state + diff < 0) {
				fprintf(err, "Error generating a curve. state = %i\n", state);
				return 0;
			}
			// record try
			int tried = ++gen_tries[state];
			if (retry && tried >= retry) {
				if (cfg->verbose) {
					fprintf(verbose, "Reached retry limit: %i, state = %i\n",
					        retry, state);
				}
				return 0;
			}

			// unroll
			for (int i = state; i > new_state;) {
				if (unrolls && unrolls[i]) {
					i += unrolls[i](curve, cfg, stack_tops[i],
					                stack_tops[i - 1]);
				} else {
					--i;
				}
			}

			if (cfg->verbose) {
				if (diff < 0) {
					fprintf(verbose, "-");
				} else {
					fprintf(verbose, ".");
				}
			}
			// unroll stack
			avma = stack_tops[new_state];
		} else {
			// we are going forward, reset tries
			gen_tries[state] = 0;
			if (cfg->verbose) fprintf(verbose, "+");
		}
		state = new_state;
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

static void exhaustive_init() { anomalous_init(); }

static void exhaustive_quit(arg_t *argss[]) {
	equation_quit();
	anomalous_quit();
	for (size_t i = 0; i < OFFSET_END; ++i) {
		if (argss[i]) {
			arg_free(&(argss[i]));
		}
	}
}

int exhaustive_do(config_t *cfg) {
	debug_log_start("Starting Exhaustive method");

	gen_t generators[OFFSET_END] = {NULL};
	arg_t *argss[OFFSET_END] = {NULL};
	unroll_t unrolls[OFFSET_END] = {NULL};
	exhaustive_ginit(generators, cfg);
	exhaustive_ainit(argss, cfg);
	exhaustive_uinit(unrolls, cfg);
	exhaustive_init();

	output_o_begin(cfg);
	for (unsigned long i = 0; i < cfg->count; ++i) {
		debug_log_start("Generating new curve");
		curve_t *curve = curve_new();
		if (!exhaustive_gen(curve, cfg, generators, argss, unrolls, OFFSET_SEED,
		                    OFFSET_END)) {
			curve_free(&curve);
			return EXIT_FAILURE;
		}
		debug_log_end("Generated new curve");

		output_o(curve, cfg);
		if (i != cfg->count - 1) {
			output_o_separator(cfg);
		}
		curve_free(&curve);
	}
	output_o_end(cfg);

	exhaustive_quit(argss);
	debug_log_end("Finished Exhaustive method");
	return EXIT_SUCCESS;
}
