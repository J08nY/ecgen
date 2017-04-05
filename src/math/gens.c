/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "gens.h"
#include "point.h"

static int gens_put(curve_t *curve, GEN generators, long len) {
	points_free_deep(&curve->generators, curve->ngens);

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

int gens_any(curve_t *curve, config_t *cfg, arg_t *args) {
	GEN generators = ellff_get_gens(curve->curve);
	long len = glength(generators);
	return gens_put(curve, generators, len);
}

int gens_one(curve_t *curve, config_t *cfg, arg_t *args) {
	pari_sp ltop = avma;
	GEN generators = ellff_get_gens(curve->curve);
	long len = glength(generators);
	if (len == 2) {
		avma = ltop;
		return -5;
	}
	return gens_put(curve, generators, len);
}
