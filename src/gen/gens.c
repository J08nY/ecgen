/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "gens.h"
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

CHECK(gens_check_cofactor) {
	return INT_MIN;
}

UNROLL(gens_unroll) {
	if (curve->generators) {
		points_free_deep(&curve->generators, curve->ngens);
	}
	return -1;
}
