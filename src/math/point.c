/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "point.h"

point_t *point_new(void) {
	point_t *point = pari_malloc(sizeof(point_t));
	if (!point) {
		perror("Couldn't malloc.");
		exit(1);
	}
	memset(point, 0, sizeof(point_t));
	return point;
}

point_t *point_copy(point_t *src, point_t *dest) {
	if (src->point) dest->point = gcopy(src->point);
	if (dest->point) dest->order = gcopy(src->order);
	return dest;
}

void point_free(point_t **point) {
	if (*point) {
		pari_free(*point);
		*point = NULL;
	}
}

point_t **points_new(size_t num) {
	point_t **points = pari_malloc(num * sizeof(point_t *));
	if (!points) {
		perror("Couldn't malloc.");
		exit(1);
	}
	memset(points, 0, num * sizeof(point_t *));
	return points;
}

point_t **points_copy(point_t **src, point_t **dest, size_t num) {
	for (size_t i = 0; i < num; ++i) {
		dest[i] = point_new();
		dest[i] = point_copy(src[i], dest[i]);
	}
	return dest;
}

void points_free(point_t ***points) {
	if (*points) {
		pari_free(*points);
		*points = NULL;
	}
}

int point_random(curve_t *curve, config_t *config, ...) {
	point_t *p = point_new();
	p->point = genrand(curve->curve);
	p->order = ellorder(curve->curve, p->point, NULL);

	curve->points = points_new(1);
	curve->points[0] = p;
	curve->npoints = 1;
	return 1;
}

int points_random(curve_t *curve, config_t *config, ...) {
	va_list arg;
	va_start(arg, config);
	size_t npoints = va_arg(arg, size_t);
	va_end(arg);

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

int points_prime(curve_t *curve, config_t *config, ...) {
	GEN factors = Z_factor(curve->order);
	GEN primes = gel(factors, 1);
	long nprimes = glength(primes);
	curve->points = points_new((size_t)nprimes);
	curve->npoints = (size_t)nprimes;

	long npoints = 0;
	while (npoints < nprimes) {
		GEN rand = genrand(curve->curve);
		GEN ord = ellorder(curve->curve, rand, NULL);
		// ord(rand) = ord

		for (long i = 1; i <= nprimes; ++i) {
			if (dvdii(ord, gel(primes, i)) && curve->points[i - 1] == NULL) {
				// primes[i] divides ord
				// mul = ord/primes[i]
				GEN mul = divii(ord, gel(primes, i));
				GEN point = ellmul(curve->curve, rand, mul);

				point_t *p = point_new();
				p->point = point;
				p->order = gel(primes, i);
				curve->points[i - 1] = p;
				npoints++;
			}
		}
	}

	return 1;
}

int points_generators(curve_t *curve, config_t *config, ...) {
	GEN generators = ellff_get_gens(curve->curve);
	long len = glength(generators);
	curve->points = points_new((size_t)len);
	curve->npoints = (size_t)len;

	for (long i = 1; i <= len; ++i) {
		point_t *p = point_new();
		p->point = gel(generators, i);
		p->order = ellorder(curve->curve, p->point, NULL);
		curve->points[i - 1] = p;
	}

	return 1;
}