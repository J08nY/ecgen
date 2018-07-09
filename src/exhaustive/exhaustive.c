/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "exhaustive.h"
#include "ansi.h"
#include "arg.h"
#include "brainpool.h"
#include "brainpool_rfc.h"
#include "check.h"
#include "gen/curve.h"
#include "gen/equation.h"
#include "gen/field.h"
#include "gen/gens.h"
#include "gen/hex.h"
#include "gen/metadata.h"
#include "gen/order.h"
#include "gen/point.h"
#include "gen/seed.h"
#include "io/output.h"
#include "misc/config.h"
#include "obj/curve.h"
#include "util/memory.h"
#include "util/timeout.h"

void exhaustive_clear(exhaustive_t *setup) {
	if (setup->validators) {
		for (size_t i = 0; i < OFFSET_END; ++i) {
			check_free(&setup->validators[i]);
		}
	}
	if (setup->gen_argss) {
		for (size_t i = 0; i < OFFSET_END; ++i) {
			arg_free(&setup->gen_argss[i]);
		}
	}
	if (setup->check_argss) {
		for (size_t i = 0; i < OFFSET_END; ++i) {
			arg_free(&setup->check_argss[i]);
		}
	}
}

static void exhaustive_ginit(gen_f *generators) {
	if (cfg->seed_algo) {
		if (cfg->prime) {
			generators[OFFSET_ORDER] = &order_gen_prime;
		} else if (cfg->cofactor) {
			generators[OFFSET_ORDER] = &order_gen_cofactor;
		} else {
			generators[OFFSET_ORDER] = &order_gen_any;
		}

		if (cfg->unique) {
			generators[OFFSET_GENERATORS] = &gens_gen_one;
		} else if (cfg->cofactor) {
			generators[OFFSET_GENERATORS] = &gens_gen_cofactor;
		} else {
			generators[OFFSET_GENERATORS] = &gens_gen_any;
		}

		switch (cfg->seed_algo) {
			case SEED_ANSI: {
				// setup ANSI X9.62 generators
				if (cfg->seed) {
					generators[OFFSET_SEED] = &ansi_gen_seed_argument;
				} else {
					if (cfg->random & RANDOM_SEED) {
						generators[OFFSET_SEED] = &ansi_gen_seed_random;
					} else {
						generators[OFFSET_SEED] = &ansi_gen_seed_input;
					}
				}
				if (cfg->random & RANDOM_FIELD) {
					generators[OFFSET_FIELD] = &field_gen_random;
				} else {
					generators[OFFSET_FIELD] = &field_gen_input;
				}
				generators[OFFSET_A] = &gen_skip;
				generators[OFFSET_B] = &ansi_gen_equation;
			} break;
			case SEED_BRAINPOOL: {
				if (cfg->seed) {
					generators[OFFSET_SEED] = &brainpool_gen_seed_argument;
				} else {
					if (cfg->random & RANDOM_SEED) {
						generators[OFFSET_SEED] = &brainpool_gen_seed_random;
					} else {
						generators[OFFSET_SEED] = &brainpool_gen_seed_input;
					}
				}
				generators[OFFSET_FIELD] = &brainpool_gen_field;
				generators[OFFSET_A] = &gen_skip;
				generators[OFFSET_B] = &brainpool_gen_equation;
				generators[OFFSET_ORDER] = &order_gen_prime;
				generators[OFFSET_GENERATORS] = &brainpool_gen_gens;
			} break;
			case SEED_BRAINPOOL_RFC: {
				if (cfg->seed) {
					generators[OFFSET_SEED] = &brainpool_rfc_gen_seed_argument;
				} else {
					if (cfg->random & RANDOM_SEED) {
						generators[OFFSET_SEED] =
						    &brainpool_rfc_gen_seed_random;
					} else {
						generators[OFFSET_SEED] = &brainpool_rfc_gen_seed_input;
					}
				}
				generators[OFFSET_FIELD] = &brainpool_gen_field;
				generators[OFFSET_A] = &gen_skip;
				generators[OFFSET_B] = &brainpool_rfc_gen_equation;
				generators[OFFSET_ORDER] = &order_gen_prime;
				generators[OFFSET_GENERATORS] = &brainpool_gen_gens;
			} break;
			case SEED_FIPS:
				break;
			default:
				break;
		}
	} else {
		// setup normal generators
		generators[OFFSET_SEED] = &gen_skip;

		if (cfg->random & RANDOM_FIELD) {
			generators[OFFSET_FIELD] = &field_gen_random;
		} else {
			generators[OFFSET_FIELD] = &field_gen_input;
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
		} else {
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
		}

		if (cfg->prime) {
			generators[OFFSET_ORDER] = &order_gen_prime;
		} else if (cfg->cofactor) {
			generators[OFFSET_ORDER] = &order_gen_cofactor;
		} else if (cfg->koblitz) {
			generators[OFFSET_ORDER] = &order_gen_koblitz;
		} else {
			generators[OFFSET_ORDER] = &order_gen_any;
		}

		if (cfg->unique) {
			generators[OFFSET_GENERATORS] = &gens_gen_one;
		} else if (cfg->cofactor) {
			generators[OFFSET_GENERATORS] = &gens_gen_cofactor;
		} else {
			generators[OFFSET_GENERATORS] = &gens_gen_any;
		}
	}

	// setup common generators
	if (cfg->method == METHOD_TWIST) {
		generators[OFFSET_CURVE] = &curve_gen_any_twist;
	} else {
		generators[OFFSET_CURVE] = &curve_gen_any;
	}

	if (cfg->metadata) {
		generators[OFFSET_METADATA] = &metadata_gen;
	} else {
		generators[OFFSET_METADATA] = &gen_skip;
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

	if (cfg->hex_check) {
		check_t *hex_check = check_new(hex_check_param, NULL);
		validators[OFFSET_POINTS] = hex_check;
	}

	if (cfg->method == METHOD_SEED) {
		switch (cfg->seed_algo) {
			case SEED_ANSI:
				break;
			case SEED_BRAINPOOL:
			case SEED_BRAINPOOL_RFC: {
				check_t *order_check = check_new(brainpool_check_order, NULL);
				validators[OFFSET_ORDER] = order_check;
				check_t *gens_check =
				    check_new(gens_check_anomalous, brainpool_check_gens, NULL);
				validators[OFFSET_GENERATORS] = gens_check;
			} break;
			case SEED_FIPS:
				break;
			default:
				break;
		}
	}
}

static void exhaustive_ainit(arg_t **gen_argss, arg_t **check_argss) {
	if (cfg->points.type == POINTS_RANDOM) {
		arg_t *points_arg = arg_new();
		points_arg->args = &cfg->points.amount;
		points_arg->nargs = 1;
		gen_argss[OFFSET_POINTS] = points_arg;
	}

	if (cfg->cofactor) {
		arg_t *order_arg = arg_new();
		arg_t *gens_arg = arg_new();
		order_arg->args = &cfg->cofactor_value;
		order_arg->nargs = 1;
		gens_arg->args = &cfg->cofactor_value;
		gens_arg->nargs = 1;
		gen_argss[OFFSET_ORDER] = order_arg;
		gen_argss[OFFSET_GENERATORS] = gens_arg;
	}

	if (cfg->hex_check) {
		arg_t *point_arg = arg_new();
		point_arg->args = cfg->hex_check;
		point_arg->nargs = 1;
		check_argss[OFFSET_POINTS] = point_arg;
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

	pari_sp stack_tops[OFFSET_END] = {avma};
	int gen_tries[OFFSET_END] = {0};

	int state = start_offset;
	while (state < end_offset) {
		stack_tops[state] = avma;

		arg_t *gen_arg = setup->gen_argss ? setup->gen_argss[state] : NULL;
		arg_t *check_arg =
		    setup->check_argss ? setup->check_argss[state] : NULL;

		int diff;
		bool timeout = false;
		timeout_start(cfg->timeout) {
			// This is not the best, but currently the best idea I have.
			diff = start_offset - state;
			timeout = true;
		}
		else {
			debug_log_state(state, "");
			diff = setup->generators[state](curve, gen_arg, (offset_e)state);
		}
		timeout_stop();
		if (diff > 0 && setup->validators && setup->validators[state]) {
			pari_sp ctop = avma;
			check_t *validator = setup->validators[state];
			for (size_t i = 0; i < validator->nchecks; ++i) {
				int new_diff =
				    validator->checks[i](curve, check_arg, (offset_e)state);
				if (new_diff <= 0) {
					diff = new_diff;
					break;
				}
			}
			avma = ctop;
		}

		int new_state = state + diff;
		if (new_state < start_offset) new_state = start_offset;

		if (diff <= 0) {
			if (diff == INT_MIN || state + diff < 0) {
				fprintf(err, "Error generating a curve. state = %i\n", state);
				return 0;
			}
			// record try
			if (retry) {
				int tried = ++gen_tries[state];
				if (tried >= retry) {
					if (cfg->verbose) {
						fprintf(verbose,
						        "Reached retry limit: %i, state = %i\n", retry,
						        state);
					}
					return 0;
				}
			}

			// unroll
			for (int i = state; i > new_state;) {
				if (setup->unrolls && setup->unrolls[i]) {
					i += setup->unrolls[i](curve, stack_tops[i],
					                       stack_tops[i - 1]);
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
	exhaustive_ainit(setup->gen_argss, setup->check_argss);
	exhaustive_uinit(setup->unrolls);
}

static void exhaustive_quit(exhaustive_t *setup) {
	field_quit();
	equation_quit();
	exhaustive_clear(setup);
}

int exhaustive_generate(exhaustive_t *setup) {
	output_o_begin();
	int result = EXIT_SUCCESS;
	for (unsigned long i = 0; i < cfg->count; ++i) {
		debug_log_start("Generating new curve");
		curve_t *curve = curve_new();
		if (!exhaustive_gen(curve, setup, OFFSET_SEED, OFFSET_END)) {
			curve_free(&curve);
			result = EXIT_FAILURE;
			break;
		}
		debug_log_end("Generated new curve");

		output_o(curve);
		if (i != cfg->count - 1) {
			output_o_separator();
		}
		curve_free(&curve);
	}
	output_o_end();

	return result;
}

int exhaustive_do() {
	debug_log_start("Starting Exhaustive method");

	gen_f generators[OFFSET_END] = {NULL};
	arg_t *gen_argss[OFFSET_END] = {NULL};
	check_t *validators[OFFSET_END] = {NULL};
	arg_t *check_argss[OFFSET_END] = {NULL};
	unroll_f unrolls[OFFSET_END] = {NULL};

	exhaustive_t setup = {.generators = generators,
	                      .gen_argss = gen_argss,
	                      .validators = validators,
	                      .check_argss = check_argss,
	                      .unrolls = unrolls};
	exhaustive_init(&setup);
	int result = exhaustive_generate(&setup);
	exhaustive_quit(&setup);
	debug_log_end("Finished Exhaustive method");
	return result;
}
