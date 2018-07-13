/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "cm.h"
#include "anomalous.h"
#include "cm_any.h"
#include "cm_prime.h"
#include "exhaustive/check.h"
#include "exhaustive/exhaustive.h"
#include "gen/curve.h"
#include "gen/field.h"
#include "gen/gens.h"
#include "gen/hex.h"
#include "gen/metadata.h"
#include "gen/point.h"
#include "obj/curve.h"
#include "supersingular.h"
#include "util/str.h"

static void cm_ginit(gen_f *generators, bool prime) {
	// SEED unused.
	generators[OFFSET_SEED] = &gen_skip;

	// Setup stuff so it can be overridden.
	if (cfg->unique) {
		generators[OFFSET_GENERATORS] = &gens_gen_one;
	} else {
		generators[OFFSET_GENERATORS] = &gens_gen_any;
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

	// Now do the actual CM setup.
	if (cfg->method == METHOD_CM) {
		generators[OFFSET_FIELD] = &gen_skip;
		generators[OFFSET_A] = &gen_skip;
		generators[OFFSET_B] = &gen_skip;
		if (prime) {
			generators[OFFSET_CURVE] = &cm_gen_curve_prime;
			generators[OFFSET_POINTS] = &points_gen_prime;
		} else {
			generators[OFFSET_CURVE] = &cm_gen_curve_any;
		}
		generators[OFFSET_ORDER] = &cm_gen_order;
	} else if (cfg->method == METHOD_ANOMALOUS) {
		generators[OFFSET_FIELD] = &anomalous_gen_field;
		generators[OFFSET_A] = &gen_skip;
		generators[OFFSET_B] = &anomalous_gen_equation;
		generators[OFFSET_CURVE] = &curve_gen_any;
		generators[OFFSET_ORDER] = &anomalous_gen_order;
	} else if (cfg->method == METHOD_SUPERSINGULAR) {
		if (cfg->random & RANDOM_FIELD) {
			generators[OFFSET_FIELD] = &field_gen_random;
		} else {
			generators[OFFSET_FIELD] = &field_gen_input;
		}
		generators[OFFSET_A] = &gen_skip;
		generators[OFFSET_B] = &supersingular_gen_equation;
		generators[OFFSET_CURVE] = &curve_gen_any;
		generators[OFFSET_ORDER] = &supersingular_gen_order;
	}
}

static void cm_ainit(arg_t **gen_argss, arg_t **check_argss,
                     const char *order_s) {
	if (cfg->method == METHOD_CM) {
		arg_t *curve_arg = arg_new();
		arg_t *order_arg = arg_new();
		char *order = try_strdup(order_s);
		curve_arg->args = order;
		curve_arg->nargs = 1;
		order_arg->args = order;
		order_arg->nargs = 1;
		order_arg->allocd = order;
		gen_argss[OFFSET_CURVE] = curve_arg;
		gen_argss[OFFSET_ORDER] = order_arg;
	}

	if (cfg->method == METHOD_ANOMALOUS) {
		arg_t *field_arg = arg_new();
		arg_t *eq_arg = arg_new();
		size_t *i = try_calloc(sizeof(size_t));
		*i = 3;
		field_arg->args = i;
		field_arg->nargs = 1;
		eq_arg->args = i;
		eq_arg->nargs = 1;
		eq_arg->allocd = i;
		gen_argss[OFFSET_FIELD] = field_arg;
		gen_argss[OFFSET_B] = eq_arg;
	}

	if (cfg->points.type == POINTS_RANDOM) {
		arg_t *points_arg = arg_new();
		points_arg->args = &cfg->points.amount;
		points_arg->nargs = 1;
		gen_argss[OFFSET_POINTS] = points_arg;
	}
}

static void cm_cinit(check_t **validators) {
	check_t *curve_check = check_new(curve_check_nonzero, NULL);
	validators[OFFSET_CURVE] = curve_check;

	if (cfg->hex_check) {
		check_t *hex_check = check_new(hex_check_param, NULL);
		validators[OFFSET_POINTS] = hex_check;
	}
}

static int cm_init(exhaustive_t *setup) {
	bool ord_prime = false;

	char *order_s = NULL;
	if (cfg->method == METHOD_CM) {
		size_t delims = str_cnt(cfg->cm_order, ',');
		GEN order;
		if (delims == 0) {
			order = strtoi(cfg->cm_order);
			order_s = cfg->cm_order;
		} else {
			GEN factors = gtovec0(gen_0, delims + 1);
			char *delim;
			char *s_start = try_strdup(cfg->cm_order);
			char *s = s_start;
			long i = 1;
			while (true) {
				if ((delim = strchr(s, ',')) != NULL) {
					*delim = 0;
					gel(factors, i++) = strtoi(s);
					s = delim + 1;
				} else {
					gel(factors, i++) = strtoi(s);
					break;
				}
			}

			order = gen_1;
			long len = glength(factors);
			for (long j = 1; j <= len; ++j) {
				GEN factor = gel(factors, j);
				if (isprime(factor)) {
					addprimes(gtovec(factor));
				} else {
					GEN factored = Z_factor(order);
					addprimes(gel(factored, 1));
				}
				order = mulii(order, factor);
			}
			try_free(s_start);
			order_s = pari_sprintf("%Pi", order);
		}
		if (gequal0(order)) {
			fprintf(err, "Order requested not a number: %s\n", cfg->cm_order);
			return 1;
		}
		long ord_log = logint0(order, gen_2, NULL);
		if (ord_log > cfg->bits) {
			pari_fprintf(err,
			             "Order requested does not fit(2^%li) into requested "
			             "bitsize(2^%li): %Pi\n",
			             ord_log, cfg->bits, order);
			return 1;
		}
		if (delims == 0) {
			ord_prime = (bool)isprime(order);
		} else {
			ord_prime = false;
		}
	}

	if (cfg->method == METHOD_ANOMALOUS) {
		anomalous_init();
	}

	cm_ginit(setup->generators, ord_prime);
	cm_ainit(setup->gen_argss, setup->check_argss, order_s);
	cm_cinit(setup->validators);
	exhaustive_uinit(setup->unrolls);

	return 0;
}

static void cm_quit(exhaustive_t *setup) {
	if (cfg->method == METHOD_ANOMALOUS) {
		anomalous_quit();
	}
	exhaustive_clear(setup);
}

int cm_do() {
	debug_log_start("Starting Complex Multiplication method");

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

	int result = cm_init(&setup);
	if (result) {
		return result;
	}

	result = exhaustive_generate(&setup);

	cm_quit(&setup);

	debug_log_start("Finished Complex Multiplication method");
	return result;
}
