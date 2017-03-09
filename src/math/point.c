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

void points_free_deep(point_t ***points, size_t npoints) {
	if (*points) {
		for (size_t i = 0; i < npoints; ++i) {
			point_free(&(*points)[i]);
		}
		points_free(points);
	}
}

int point_random(curve_t *curve, config_t *cfg, arg_t *args) {
	points_free_deep(&curve->points, curve->npoints);

	point_t *p = point_new();
	p->point = genrand(curve->curve);
	p->order = ellorder(curve->curve, p->point, NULL);

	curve->points = points_new(1);
	curve->points[0] = p;
	curve->npoints = 1;
	return 1;
}

int points_random(curve_t *curve, config_t *cfg, arg_t *args) {
	if (!args) {
		fprintf(stderr, "No args to an arged function. points_random");
		return INT_MIN;
	}
	points_free_deep(&curve->points, curve->npoints);

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

/*
    GEN o = utoi(dprimes[i]);
    GEN mul = ellmul(curve->curve, rand, o);

    if (gequal0(mul)) {
        printf("Success! %lu\n", npoints);
        curve->points[i] = point_new();

        gerepileall(btop, 2, &rand, &o);
        curve->points[i]->point = rand;
        curve->points[i]->order = o;
        npoints++;
        break;
    }
 */

int points_trial(curve_t *curve, config_t *cfg, arg_t *args) {
	// TODO stack code!!!
	if (!args) {
		fprintf(stderr, "No args to an arged function. points_trial");
		return INT_MIN;
	}
	points_free_deep(&curve->points, curve->npoints);

	pari_ulong *primes = (pari_ulong *)args->args;
	size_t nprimes = args->nargs;

	curve->points = points_new(nprimes);
	curve->npoints = nprimes;

	size_t npoints = 0;
	while (npoints < nprimes) {
		GEN rand = genrand(curve->curve);
		GEN ord = ellorder(curve->curve, rand, NULL);

		for (long i = 0; i < nprimes; ++i) {
			if (curve->points[i] == NULL && dvdis(ord, primes[i])) {
				pari_sp ftop = avma;

				GEN p = stoi(primes[i]);
				GEN mul = divii(ord, p);
				GEN point = ellmul(curve->curve, rand, mul);

				curve->points[i] = point_new();
				gerepileall(ftop, 2, &point, &p);
				curve->points[i]->point = point;
				curve->points[i]->order = p;
				npoints++;
			}
		}
	}

	return 1;
}

int points_prime(curve_t *curve, config_t *cfg, arg_t *args) {
	// TODO stack code!!!
	points_free_deep(&curve->points, curve->npoints);

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
			if (curve->points[i - 1] == NULL && dvdii(ord, gel(primes, i))) {
				pari_sp ftop = avma;

				// primes[i] divides ord
				// mul = ord/primes[i]
				GEN p = gcopy(gel(primes, i));
				GEN mul = divii(ord, p);
				GEN point = ellmul(curve->curve, rand, mul);

				curve->points[i - 1] = point_new();
				gerepileall(ftop, 2, &point, &p);
				curve->points[i - 1]->point = point;
				curve->points[i - 1]->order = p;
				npoints++;
			}
		}
	}

	return 1;
}
