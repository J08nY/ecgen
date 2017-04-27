/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "curve.h"
#include "exhaustive/seed.h"
#include "field.h"
#include "point.h"

curve_t *curve_new(void) {
	curve_t *curve = pari_malloc(sizeof(curve_t));
	if (!curve) {
		perror("Couldn't malloc.");
		exit(1);
	}
	memset(curve, 0, sizeof(curve_t));
	return curve;
}

curve_t *curve_copy(const curve_t *src, curve_t *dest) {
	if (src->seed) dest->seed = seed_copy(src->seed, dest->seed);
	if (src->field) dest->field = gcopy(src->field);
	if (src->a) dest->a = gcopy(src->a);
	if (src->b) dest->b = gcopy(src->b);
	if (src->curve) dest->curve = gcopy(src->curve);
	if (src->order) dest->order = gcopy(src->order);
	if (src->generators) {
		dest->generators = points_new_copy(src->generators, src->ngens);
		dest->ngens = src->ngens;
	}
	if (src->points) {
		dest->points = points_new_copy(src->points, src->npoints);
		dest->npoints = src->npoints;
	}
	return dest;
}

curve_t *curve_new_copy(const curve_t *src) {
	curve_t *result = curve_new();
	return curve_copy(src, result);
}

curve_t *curve_clone(const curve_t *src, curve_t *dest) {
	if (src->seed) dest->seed = seed_clone(src->seed, dest->seed);
	if (src->field) dest->field = gclone(src->field);
	if (src->a) dest->a = gclone(src->a);
	if (src->b) dest->b = gclone(src->b);
	if (src->curve) dest->curve = gclone(src->curve);
	if (src->order) dest->order = gclone(src->order);
	if (src->generators) {
		dest->generators = points_new_clone(src->generators, src->ngens);
		dest->ngens = src->ngens;
	}
	if (src->points) {
		dest->points = points_new_clone(src->points, src->npoints);
		dest->npoints = src->npoints;
	}
	return dest;
}

curve_t *curve_new_clone(const curve_t *src) {
	curve_t *result = curve_new();
	return curve_clone(src, result);
}

void curve_free(curve_t **curve) {
	if (*curve) {
		seed_free(&(*curve)->seed);

		if ((*curve)->curve) {
			// TODO, this is possibly dangerous...
			obj_free((*curve)->curve);
			if (isclone((*curve)->curve)) {
				gunclone((*curve)->curve);
			}
		}

		if ((*curve)->field && isclone((*curve)->field)) {
			gunclone((*curve)->field);
		}
		if ((*curve)->a && isclone((*curve)->a)) {
			gunclone((*curve)->a);
		}
		if ((*curve)->b && isclone((*curve)->b)) {
			gunclone((*curve)->b);
		}
		if ((*curve)->order && isclone((*curve)->order)) {
			gunclone((*curve)->order);
		}

		points_free_deep(&(*curve)->generators, (*curve)->ngens);
		points_free_deep(&(*curve)->points, (*curve)->npoints);
		pari_free(*curve);
		*curve = NULL;
	}
}

int curve_any(curve_t *curve, const config_t *cfg, arg_t *args) {
	pari_sp ltop = avma;
	GEN v = gen_0;
	switch (typ(curve->field)) {
		case t_INT:
			v = gtovec0(gen_0, 2);
			gel(v, 1) = curve->a;
			gel(v, 2) = curve->b;
			break;
		case t_FFELT:
			v = gtovec0(gen_0, 5);
			gel(v, 1) = gen_1;
			gel(v, 2) = curve->a;
			gel(v, 4) = curve->b;
			break;
		default:
			pari_err_TYPE("curve_any", curve->field);
	}
	GEN crv = ellinit(v, curve->field, -1);

	if (glength(crv) == 0) {
		avma = ltop;
		return -3;
	} else {
		curve->curve = gerepilecopy(ltop, crv);
		return 1;
	}
}

int curve_nonzero(curve_t *curve, const config_t *cfg, arg_t *args) {
	pari_sp ltop = avma;
	int any = curve_any(curve, cfg, args);
	if (any <= 0) {
		return any;
	}
	if (gequal0(ell_get_disc(curve->curve))) {
		avma = ltop;
		return -3;
	} else {
		return 1;
	}
}

static int curve_seed_fp(curve_t *curve, const config_t *cfg, arg_t *args) {
	// TODO implement
	return INT_MIN;
}

static int curve_seed_f2m(curve_t *curve, const config_t *cfg, arg_t *args) {
	// TODO implement
	return INT_MIN;
}

int curve_seed(curve_t *curve, const config_t *cfg, arg_t *args) {
	switch (typ(curve->field)) {
		case t_INT:
			return curve_seed_fp(curve, cfg, args);
		case t_FFELT:
			return curve_seed_f2m(curve, cfg, args);
		default:
			pari_err_TYPE("curve_seed", curve->field);
			return INT_MIN; /* NOT REACHABLE */
	}
}

int curve_unroll(curve_t *curve, const config_t *cfg, pari_sp from,
                 pari_sp to) {
	if (curve->curve) {
		obj_free(curve->curve);
		curve->curve = NULL;
	}
	return -1;
}

GEN curve_params(const curve_t *curve) {
	pari_sp ltop = avma;

	GEN result = field_params(curve->field);
	if (curve->a) result = gconcat(result, field_elementi(curve->a));
	if (curve->b) result = gconcat(result, field_elementi(curve->b));
	if (curve->generators) {
		for (size_t i = 0; i < curve->ngens; ++i) {
			GEN x = field_elementi(gel(curve->generators[i]->point, 1));
			GEN y = field_elementi(gel(curve->generators[i]->point, 2));
			result = gconcat(result, x);
			result = gconcat(result, y);
			result = gconcat(result, curve->generators[i]->order);
			if (curve->generators[i]->cofactor) {
				result = gconcat(result, curve->generators[i]->cofactor);
			}
		}
	}
	if (curve->order) result = gconcat(result, gtovec(curve->order));
	if (curve->points) {
		for (size_t i = 0; i < curve->npoints; ++i) {
			GEN x = field_elementi(gel(curve->points[i]->point, 1));
			GEN y = field_elementi(gel(curve->points[i]->point, 2));
			result = gconcat(result, x);
			result = gconcat(result, y);
			result = gconcat(result, curve->points[i]->order);
			if (curve->points[i]->cofactor) {
				result = gconcat(result, curve->points[i]->cofactor);
			}
		}
	}

	return gerepilecopy(ltop, result);
}
