/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "point.h"
#include "io/output.h"
#include "math/subgroups.h"
#include "util/memory.h"

point_t *point_new(void) { return try_calloc(sizeof(point_t)); }

point_t *point_copy(const point_t *src, point_t *dest) {
	if (src->point) dest->point = gcopy(src->point);
	if (src->order) dest->order = gcopy(src->order);
	if (src->cofactor) dest->cofactor = gcopy(src->cofactor);
	return dest;
}

point_t *point_new_copy(const point_t *src) {
	point_t *result = point_new();
	return point_copy(src, result);
}

point_t *point_clone(const point_t *src, point_t *dest) {
	if (src->point) dest->point = gclone(src->point);
	if (src->order) dest->order = gclone(src->order);
	if (src->cofactor) dest->cofactor = gclone(src->cofactor);
	return dest;
}

point_t *point_new_clone(const point_t *src) {
	point_t *result = point_new();
	return point_clone(src, result);
}

void point_free(point_t **point) {
	if (*point) {
		if ((*point)->point && isclone((*point)->point)) {
			gunclone((*point)->point);
		}
		if ((*point)->order && isclone((*point)->order)) {
			gunclone((*point)->order);
		}
		if ((*point)->cofactor && isclone((*point)->cofactor)) {
			gunclone((*point)->cofactor);
		}
		try_free(*point);
		*point = NULL;
	}
}

point_t **points_new(size_t num) { return try_calloc(num * sizeof(point_t *)); }

point_t **points_copy(point_t **const src, point_t **dest, size_t num) {
	for (size_t i = 0; i < num; ++i) {
		dest[i] = point_new_copy(src[i]);
	}
	return dest;
}

point_t **points_new_copy(point_t **const src, size_t num) {
	point_t **result = points_new(num);
	return points_copy(src, result, num);
}

point_t **points_clone(point_t **const src, point_t **dest, size_t num) {
	for (size_t i = 0; i < num; ++i) {
		dest[i] = point_new_clone(src[i]);
	}
	return dest;
}

point_t **points_new_clone(point_t **const src, size_t num) {
	point_t **result = points_new(num);
	return points_clone(src, result, num);
}

void points_free(point_t ***points) {
	if (*points) {
		try_free(*points);
		*points = NULL;
	}
}

void points_free_deep(point_t ***points, size_t npoints) {
	if (*points) {
		for (size_t i = 0; i < npoints; ++i) {
			point_free(&(*points)[i]);
		}
		points_free(points);
	}
}

GENERATOR(point_gen_random) {
	point_t *p = point_new();
	p->point = genrand(curve->curve);
	p->order = ellorder(curve->curve, p->point, NULL);

	curve->points = points_new(1);
	curve->points[0] = p;
	curve->npoints = 1;
	return 1;
}

GENERATOR(points_gen_random) {
	if (!args) {
		fprintf(err, "No args to an arged function. points_gen_random\n");
		return INT_MIN;
	}

	size_t npoints = *(size_t *)args->args;

	curve->points = points_new(npoints);
	curve->npoints = npoints;
	for (size_t i = 0; i < npoints; ++i) {
		point_t *p = point_new();
		p->point = genrand(curve->curve);
		p->order = ellorder(curve->curve, p->point, NULL);
		curve->points[i] = p;
	}
	return 1;
}

static int points_from_orders(curve_t *curve, GEN orders) {
	// TODO better stack code
	size_t norders = (size_t)glength(orders);

	curve->points = points_new(norders);
	curve->npoints = norders;

	for (size_t ngen = 0; ngen < curve->ngens; ++ngen) {
		point_t *gen = curve->generators[ngen];

		for (long i = 0; i < norders; ++i) {
			GEN num = gel(orders, i + 1);
			if (curve->points[i] == NULL) {
				pari_sp ftop = avma;

				GEN point = NULL;
				if (equalii(gen->order, num)) {
					point = gcopy(gen->point);
				} else if (dvdii(gen->order, num)) {
					GEN mul = divii(gen->order, num);
					point = ellmul(curve->curve, gen->point, mul);
				}

				if (point) {
					debug_log("VERIFY %Ps %Ps", num,
					          ellorder(curve->curve, point, NULL));
					curve->points[i] = point_new();
					gerepileall(ftop, 1, &point);
					curve->points[i]->point = point;
					curve->points[i]->order = gcopy(num);
				}
			}
		}
	}

	return 1;
}

GENERATOR(points_gen_trial) {
	if (!args) {
		fprintf(err, "No args to an arged function. points_gen_trial\n");
		return INT_MIN;
	}

	pari_ulong *primes = (pari_ulong *)args->args;
	size_t nprimes = args->nargs;

	GEN orders = gtovec0(gen_0, nprimes);
	for (size_t i = 1; i <= nprimes; ++i) {
		gel(orders, i) = utoi(primes[i - 1]);
	}

	return points_from_orders(curve, orders);
}

GENERATOR(points_gen_prime) {
	GEN primes = subgroups_prime(curve, cfg);
	return points_from_orders(curve, primes);
}

GENERATOR(points_gen_allgroups) {
	GEN groups = subgroups_all(curve, cfg);
	return points_from_orders(curve, groups);
}

GENERATOR(points_gen_nonprime) {
	GEN groups = subgroups_nonprime(curve, cfg);
	if (!groups) {
		// No non-prime order points on curve.
		return 1;
	} else {
		return points_from_orders(curve, groups);
	}
}

UNROLL(points_unroll) {
	if (curve->points) {
		points_free_deep(&curve->points, curve->npoints);
	}
	return -1;
}
