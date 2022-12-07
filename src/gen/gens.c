/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "gens.h"
#include "exhaustive/arg.h"
#include "math/subgroup.h"
#include "obj/point.h"
#include "obj/subgroup.h"
#include "misc/compat.h"

static subgroup_t *gens_point(GEN point, const curve_t *curve) {
	subgroup_t *sub = subgroup_new();
	point_t *p = point_new();
	sub->generator = p;
	p->point = gcopy(point);
	p->order = ellorder(curve->curve, p->point, NULL);
	p->cofactor = divii(curve->order, p->order);
	return sub;
}

static int gens_put(curve_t *curve, GEN generators, long len) {
	curve->generators = subgroups_new((size_t)len);
	curve->ngens = (size_t)len;

	for (long i = 1; i <= len; ++i) {
		curve->generators[i - 1] = gens_point(gel(generators, i), curve);
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

GENERATOR(gens_gen_cofactor) {
	HAS_ARG(args);
	pari_ulong cofactor = *(pari_ulong *)args->args;
	pari_sp ltop = avma;
	GEN order = diviuexact(curve->order, cofactor);

	GEN generators = ellff_get_gens(curve->curve);
	long len = glength(generators);
	point_t *p = NULL;
	for (long i = 1; i <= len; ++i) {
		GEN gen = gel(generators, i);
		GEN gen_order = ellorder(curve->curve, gen, NULL);

		if (equalii(order, gen_order)) {
			p = point_new();
			p->point = gcopy(gen);
			p->order = gen_order;
			p->cofactor = utoi(cofactor);
			break;
		}

		if (dvdiu(gen_order, cofactor)) {
			GEN res = diviuexact(gen_order, cofactor);
			p = point_new();
			p->point = gcopy(ellmul(curve->curve, gen, utoi(cofactor)));
			p->order = res;
			p->cofactor = utoi(cofactor);
			break;
		}
	}

	if (p) {
		curve->ngens = (size_t)(len + 1);
		curve->generators = subgroups_new(curve->ngens);
		for (long i = 1; i <= len; ++i) {
			curve->generators[i] = gens_point(gel(generators, i), curve);
		}
		subgroup_t *sub = subgroup_new();
		sub->generator = p;
		curve->generators[0] = sub;
		return 1;
	} else {
		avma = ltop;
		return -5;
	}
}

CHECK(gens_check_anomalous) {
	if (cfg->field == FIELD_BINARY) return 1;
	for (size_t i = 0; i < curve->ngens; ++i) {
		if (mpcmp(curve->field, curve->generators[i]->generator->order) == 0) {
			return -5;
		}
	}
	return 1;
}

GEN gens_get_embedding(GEN prime, GEN generator_order) {
	return znorder(mkintmod(prime, generator_order), NULL);
}

CHECK(gens_check_embedding) {
	HAS_ARG(args);
	if (cfg->field == FIELD_BINARY) return 1;
	pari_sp ltop = avma;

	const char *min_degree = args->args;
	GEN mind = strtoi(min_degree);

	for (size_t i = 0; i < curve->ngens; ++i) {
		GEN power = gens_get_embedding(curve->field,
		                               curve->generators[i]->generator->order);

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
		subgroups_free_deep(&curve->generators, curve->ngens);
	}
	return -1;
}
