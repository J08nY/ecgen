/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file point.h
 */
#ifndef ECGEN_GEN_POINT_H
#define ECGEN_GEN_POINT_H

#include "misc/types.h"

/**
 * GENERATOR(gen_f)
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(point_gen_random);

/**
 * GENERATOR(gen_f)
 *
 * @param curve A curve_t being generated
 * @param args size_t number of points to generate
 * @return state diff
 */
GENERATOR(points_gen_random);

/**
 *
 * @param curve
 * @param generator
 * @param orders
 * @return
 */
point_t **points_from_orders(GEN curve, point_t *generator, GEN orders);

/**
 * GENERATOR(gen_f)
 * Generates prime order points using trial division.
 *
 * Assumes the primes divide curve order, thus that points with all
 * prime orders specified exist.
 *
 * @param curve A curve_t being generated
 * @param args pari_ulong array of primes length nargs
 * @return state diff
 */
GENERATOR(points_gen_trial);

/**
 * GENERATOR(gen_f)
 *
 * Cauchy:
 *   Let G be a finite group and p be a prime. If p divides the order of G, then
 *   G has an element of order p.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(points_gen_prime);

/**
 * GENERATOR(gen_f)
 *
 * Generates points on all subgroups of the curve. Prime and non-prime order.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(points_gen_allgroups);

/**
 * GENERATOR(gen_f)
 *
 * Generates points on non-prime order of the curve.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(points_gen_nonprime);

/**
 * UNROLL(unroll_f)
 *
 * @param curve
 * @param from
 * @param to
 * @return
 */
UNROLL(points_unroll);

#endif  // ECGEN_GEN_POINT_H
