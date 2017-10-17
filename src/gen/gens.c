/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "gens.h"
#include "exhaustive/arg.h"
#include "point.h"

static int gens_put(curve_t *curve, GEN generators, long len) {
	curve->generators = points_new((size_t)len);
	curve->ngens = (size_t)len;

	for (long i = 1; i <= len; ++i) {
		point_t *p = point_new();
		p->point = gel(generators, i);
		p->order = ellorder(curve->curve, p->point, NULL);
		p->cofactor = divii(curve->order, p->order);
		curve->generators[i - 1] = p;
	}

	return 1;
}

GENERATOR(gens_gen_any) {
	GEN generators = ellff_get_gens(curve->curve);
	long len = glength(generators);
	return gens_put(curve, generators, len);
}

GENERATOR(gens_gen_one) {
	pari_sp ltop = avma;
	GEN generators = ellff_get_gens(curve->curve);
	long len = glength(generators);
	if (len == 2) {
		avma = ltop;
		return -5;
	}
	return gens_put(curve, generators, len);
}

CHECK(gens_check_anomalous) {
	if (cfg->field == FIELD_BINARY) return 1;
	pari_sp ltop = avma;
	for (size_t i = 0; i < curve->ngens; ++i) {
		if (mpcmp(curve->field, curve->generators[i]->order) == 0) {
			avma = ltop;
			return -5;
		}
	}
	return 1;
}

CHECK(gens_check_embedding) {
	HAS_ARG(args);
	if (cfg->field == FIELD_BINARY) return 1;
	pari_sp ltop = avma;

	const char *min_degree = args->args;
	GEN mind = strtoi(min_degree);

	for (size_t i = 0; i < curve->ngens; ++i) {
		GEN power = gen_0;
		GEN pm;
		do {
			power = addii(power, gen_1);
			GEN ppow = powii(curve->field, power);
			pm = subii(ppow, gen_1);
		} while (!dvdii(pm, curve->generators[i]->order));

		if (mpcmp(power, mind) <= 0) {
			avma = ltop;
			return -5;
		}
	}
	avma = ltop;
	return 1;
}

UNROLL(gens_unroll) {
	if (curve->generators) {
		points_free_deep(&curve->generators, curve->ngens);
	}
	return -1;
}
