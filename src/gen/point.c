/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "point.h"
#include "exhaustive/arg.h"
#include "math/subgroup.h"
#include "obj/point.h"
#include "util/random.h"

GENERATOR(point_gen_random) {
	long which_gen = itos(random_range(gen_0, stoi(curve->ngens)));

	subgroup_t *subgroup = curve->generators[which_gen];
	GEN mul = random_range(gen_0, subgroup->generator->order);
	GEN p = ellmul(curve->curve, subgroup->generator->point, mul);
	point_t *point = point_new();
	point->point = p;
	point->order = ellorder(curve->curve, p, NULL);
	subgroup->npoints = 1;
	subgroup->points = points_new(1);
	subgroup->points[0] = point;
	return 1;
}

GENERATOR(points_gen_random) {
	HAS_ARG(args);

	size_t npoints = *(size_t *)args->args;
	size_t npoints_per_gen[curve->ngens];

	for (size_t i = 0; i < curve->ngens; ++i) {
		npoints_per_gen[i] = 0;
	}

	for (size_t i = 0; i < npoints; ++i) {
		long which_gen = itos(random_range(gen_0, stoi(curve->ngens)));
		npoints_per_gen[which_gen]++;
	}

	for (size_t i = 0; i < curve->ngens; ++i) {
		subgroup_t *subgroup = curve->generators[i];
		subgroup->npoints = npoints_per_gen[i];
		subgroup->points = points_new(npoints_per_gen[i]);

		for (size_t j = 0; j < npoints_per_gen[i]; ++j) {
			point_t *point = point_new();
			// Handle the special case of subgroup of order 2.
			if (equalis(subgroup->generator->order, 2)) {
				point->point = gcopy(subgroup->generator->point);
				point->order = stoi(2);
			} else {
				GEN mul = random_range(gen_1, subgroup->generator->order);
				GEN p = ellmul(curve->curve, subgroup->generator->point, mul);
				point->point = p;
				point->order = ellorder(curve->curve, p, NULL);
			}
			subgroup->points[j] = point;
		}
	}
	return 1;
}

point_t **points_from_orders(GEN curve, point_t *generator, GEN orders) {
	size_t norders = (size_t)glength(orders);
	point_t **result = points_new(norders);

	for (long i = 0; i < norders; ++i) {
		pari_sp ftop = avma;
		GEN num = gel(orders, i + 1);

		GEN point = NULL;
		if (equalii(generator->order, num)) {
			point = gcopy(generator->point);
		} else if (dvdii(generator->order, num)) {
			GEN mul = divii(generator->order, num);
			point = ellmul(curve, generator->point, mul);
		}

		if (point) {
			debug_log("VERIFY %Ps %Ps", num, ellorder(curve, point, NULL));
			result[i] = point_new();
			gerepileall(ftop, 1, &point);
			result[i]->point = point;
			result[i]->order = gcopy(num);
		}
	}

	return result;
}

GENERATOR(points_gen_trial) {
	HAS_ARG(args);

	pari_ulong *primes = (pari_ulong *)args->args;
	size_t nprimes = args->nargs;

	GEN orders = gtovec0(gen_0, nprimes);
	for (size_t i = 1; i <= nprimes; ++i) {
		gel(orders, i) = utoi(primes[i - 1]);
	}

	GEN orders_per_gen[curve->ngens];

	for (size_t i = 0; i < curve->ngens; ++i) {
		orders_per_gen[i] = gen_0;
	}

	for (size_t j = 0; j < nprimes; ++j) {
		GEN num = gel(orders, j + 1);

		for (size_t i = 0; i < curve->ngens; ++i) {
			point_t *gen = curve->generators[i]->generator;
			if (equalii(gen->order, num) || dvdii(gen->order, num)) {
				if (orders_per_gen[i] == gen_0) {
					orders_per_gen[i] = gtovec(num);
				} else {
					vec_append(orders_per_gen[i], num);
				}
			}
		}
	}

	for (size_t i = 0; i < curve->ngens; ++i) {
		subgroup_t *subgroup = curve->generators[i];
		if (orders_per_gen[i] != gen_0) {
			subgroup->npoints = (size_t)glength(orders_per_gen[i]);
			subgroup->points = points_from_orders(
			    curve->curve, subgroup->generator, orders_per_gen[i]);
		}
	}

	return 1;
}

GENERATOR(points_gen_prime) {
	for (size_t i = 0; i < curve->ngens; ++i) {
		GEN primes = subgroups_prime(curve->generators[i]->generator->order);
		curve->generators[i]->npoints = (size_t)glength(primes);
		curve->generators[i]->points = points_from_orders(
		    curve->curve, curve->generators[i]->generator, primes);
	}

	return 1;
}

GENERATOR(points_gen_allgroups) {
	for (size_t i = 0; i < curve->ngens; ++i) {
		GEN primes = subgroups_all(curve->generators[i]->generator->order);
		curve->generators[i]->npoints = (size_t)glength(primes);
		curve->generators[i]->points = points_from_orders(
		    curve->curve, curve->generators[i]->generator, primes);
	}

	return 1;
}

GENERATOR(points_gen_nonprime) {
	for (size_t i = 0; i < curve->ngens; ++i) {
		GEN primes = subgroups_nonprime(curve->generators[i]->generator->order);
		if (primes) {
			curve->generators[i]->npoints = (size_t)glength(primes);
			curve->generators[i]->points = points_from_orders(
			    curve->curve, curve->generators[i]->generator, primes);
		}
	}

	return 1;
}

UNROLL(points_unroll) {
	if (curve->generators) {
		for (size_t i = 0; i < curve->ngens; ++i) {
			points_free_deep(&curve->generators[i]->points,
			                 curve->generators[i]->npoints);
		}
	}
	return -1;
}
