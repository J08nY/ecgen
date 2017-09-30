/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "gp.h"
#include "exhaustive/arg.h"
#include "io/output.h"
#include "point.h"
#include "seed.h"
#include "util/bits.h"

static point_t **gp_points(const curve_t *curve, GEN point_vec) {
	long len = glength(point_vec);
	point_t **result = points_new((size_t)len);

	for (long i = 1; i <= len; ++i) {
		point_t *point = point_new();
		point->point = gel(point_vec, i);
		point->order = ellorder(curve->curve, point->point, NULL);
		result[i - 1] = point;
	}
	return result;
}

static point_t **gp_gens(const curve_t *curve, GEN gens_vec) {
	point_t **result = gp_points(curve, gens_vec);

	long len = glength(gens_vec);
	for (long i = 1; i <= len; ++i) {
		point_t *gen = result[i - 1];
		gen->cofactor = divii(curve->order, gen->order);
	}
	return result;
}

GENERATOR(gp_gen) {
	HAS_ARG(args);
	pari_sp ltop = avma;
	GEN closure = compile_str(args->args);
	GEN params = zerovec(state - OFFSET_SEED);

	if (state > OFFSET_SEED) {
		if (curve->seed && curve->seed->seed) {
			gel(params, 1) = bits_to_bitvec(curve->seed->seed);
		}
	}

	if (state > OFFSET_FIELD) {
		gel(params, 2) = curve->field;
	}

	if (state > OFFSET_A) {
		gel(params, 3) = curve->a;
	}

	if (state > OFFSET_B) {
		gel(params, 4) = curve->b;
	}

	if (state > OFFSET_CURVE) {
		gel(params, 5) = curve->curve;
	}

	if (state > OFFSET_ORDER) {
		gel(params, 6) = curve->order;
	}

	if (state > OFFSET_GENERATORS) {
		GEN gens = zerovec(curve->ngens);
		for (size_t i = 0; i < curve->ngens; ++i) {
			gel(gens, i + 1) = curve->generators[i]->point;
		}
		gel(params, 7) = gens;
	}

	if (state > OFFSET_POINTS) {
		GEN points = zerovec(curve->npoints);
		for (size_t i = 0; i < curve->npoints; ++i) {
			gel(points, i + 1) = curve->points[i]->point;
		}
		gel(params, 8) = points;
	}

	GEN res = call0(closure, zerovec(0));
	res = call0(res, params);

	res = gerepileupto(ltop, res);
	switch (state) {
		case OFFSET_SEED:
			curve->seed = seed_new();
			curve->seed->seed = bits_from_bitvec(res);
			break;
		case OFFSET_FIELD:
			curve->field = res;
			break;
		case OFFSET_A:
			curve->a = res;
			break;
		case OFFSET_B:
			curve->b = res;
			break;
		case OFFSET_CURVE:
			curve->curve = res;
			break;
		case OFFSET_ORDER:
			curve->order = res;
			break;
		case OFFSET_GENERATORS:
			curve->ngens = (size_t)glength(res);
			curve->generators = gp_gens(curve, res);
			break;
		case OFFSET_POINTS:
			curve->npoints = (size_t)glength(res);
			curve->points = gp_points(curve, res);
			break;
		case OFFSET_END:
			break;
	}
	return 1;
}