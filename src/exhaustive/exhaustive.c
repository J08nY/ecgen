/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "exhaustive.h"
#include "anomalous.h"
#include "ansi.h"
#include "check.h"
#include "gen/curve.h"
#include "gen/equation.h"
#include "gen/field.h"
#include "gen/gens.h"
#include "gen/order.h"
#include "gen/point.h"
#include "gen/seed.h"
#include "io/output.h"
#include "util/memory.h"
#include "util/timeout.h"

void exhaustive_clear(exhaustive_t *setup) {
	if (setup->validators) {
		for (size_t i = 0; i < OFFSET_END; ++i) {
			check_free(&setup->validators[i]);
		}
	}
	if (setup->argss) {
		for (size_t i = 0; i < OFFSET_END; ++i) {
			arg_free(&setup->argss[i]);
		}
	}
}

static void exhaustive_ginit(gen_f *generators) {
	if (cfg->seed_algo) {
		switch (cfg->seed_algo) {
			case SEED_ANSI: {
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
			}
			case SEED_BRAINPOOL:
				break;
			case SEED_BRAINPOOL_RFC:
				break;
			case SEED_FIPS:
				break;
			default:
				break;
		}
		if (cfg->prime) {
			generators[OFFSET_ORDER] = &order_gen_prime;
		} else if (cfg->cofactor) {
			generators[OFFSET_ORDER] = &order_gen_smallfact;
		} else {
			generators[OFFSET_ORDER] = &order_gen_any;
		}
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
	generators[OFFSET_CURVE] = &curve_gen_any;

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

static void exhaustive_cinit(check_t **validators) {
	check_t *curve_check = check_new(curve_check_nonzero, NULL);
	validators[OFFSET_CURVE] = curve_check;
}

static void exhaustive_ainit(arg_t **argss) {
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

void exhaustive_uinit(unroll_f *unrolls) {
	if (cfg->seed_algo) {
		unrolls[OFFSET_SEED] = &seed_unroll;
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

int exhaustive_gen_retry(curve_t *curve, const exhaustive_t *setup,
                         offset_e start_offset, offset_e end_offset,
                         int retry) {
	if (start_offset == end_offset) {
		return 2;
	}
	if (start_offset > end_offset) {
		return 0;
	}
	gen_f *generators = setup->generators;
	check_t **validators = setup->validators;
	arg_t **argss = setup->argss;
	unroll_f *unrolls = setup->unrolls;

	pari_sp stack_tops[OFFSET_END] = {0};
	int gen_tries[OFFSET_END] = {0};

	int state = start_offset;
	while (state < end_offset) {
		stack_tops[state] = avma;

		arg_t *arg = argss ? argss[state] : NULL;

		int diff;
		bool timeout = false;
		timeout_start(cfg->timeout) {
			// This is not the best, but currently the best idea I have.
			diff = start_offset - state;
			timeout = true;
		}
		else {
			diff = generators[state](curve, arg, (offset_e)state);
		}
		timeout_stop();
		int new_state = state + diff;
		if (new_state < start_offset) new_state = start_offset;

		if (diff > 0 && validators && validators[state]) {
			check_t *validator = validators[state];
			for (size_t i = 0; i < validator->nchecks; ++i) {
				int new_diff =
				    validator->checks[i](curve, arg, (offset_e)state);
				if (new_diff <= 0) {
					diff = new_diff;
					break;
				}
			}
		}

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
					i += unrolls[i](curve, stack_tops[i], stack_tops[i - 1]);
				} else {
					--i;
				}
			}

			if (timeout) {
				verbose_log("#");
			} else {
				if (diff < 0) {
					verbose_log("-");
				} else {
					verbose_log(".");
				}
			}
			// unroll stack
			avma = stack_tops[new_state];
		} else {
			// we are going forward, reset tries
			gen_tries[state] = 0;
			verbose_log("+");
		}
		state = new_state;
	}

	verbose_log("\n");

	return 1;
}

int exhaustive_gen(curve_t *curve, const exhaustive_t *setup,
                   offset_e start_offset, offset_e end_offset) {
	return exhaustive_gen_retry(curve, setup, start_offset, end_offset, 0);
}

static void exhaustive_init(exhaustive_t *setup) {
	exhaustive_ginit(setup->generators);
	exhaustive_cinit(setup->validators);
	exhaustive_ainit(setup->argss);
	exhaustive_uinit(setup->unrolls);
	anomalous_init();
}

static void exhaustive_quit(exhaustive_t *setup) {
	equation_quit();
	anomalous_quit();
	exhaustive_clear(setup);
}

int exhaustive_do() {
	debug_log_start("Starting Exhaustive method");

	gen_f generators[OFFSET_END] = {NULL};
	check_t *validators[OFFSET_END] = {NULL};
	arg_t *argss[OFFSET_END] = {NULL};
	unroll_f unrolls[OFFSET_END] = {NULL};

	exhaustive_t setup = {.generators = generators,
	                      .validators = validators,
	                      .argss = argss,
	                      .unrolls = unrolls};
	exhaustive_init(&setup);

	output_o_begin();
	for (unsigned long i = 0; i < cfg->count; ++i) {
		debug_log_start("Generating new curve");
		curve_t *curve = curve_new();
		if (!exhaustive_gen(curve, &setup, OFFSET_SEED, OFFSET_END)) {
			curve_free(&curve);
			return EXIT_FAILURE;
		}
		debug_log_end("Generated new curve");

		output_o(curve);
		if (i != cfg->count - 1) {
			output_o_separator();
		}
		curve_free(&curve);
	}
	output_o_end();

	exhaustive_quit(&setup);
	debug_log_end("Finished Exhaustive method");
	return EXIT_SUCCESS;
}
