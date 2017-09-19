/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file point.h
 */
#ifndef ECGEN_POINT_H
#define ECGEN_POINT_H

#include "misc/types.h"

/**
 *
 * @return
 */
point_t *point_new(void);

/**
 *
 * @param src
 * @param dest
 * @return
 */
point_t *point_copy(const point_t *src, point_t *dest);

/**
 *
 * @param src
 * @return
 */
point_t *point_new_copy(const point_t *src);

/**
 *
 * @param src
 * @param dest
 * @return
 */
point_t *point_clone(const point_t *src, point_t *dest);

/**
 *
 * @param src
 * @return
 */
point_t *point_new_clone(const point_t *src);

/**
 *
 * @param point
 */
void point_free(point_t **point);

/**
 *
 * @param num
 * @return
 */
point_t **points_new(size_t num);

/**
 *
 * @param src
 * @param dest
 * @param num
 * @return
 */
point_t **points_copy(point_t **const src, point_t **dest, size_t num);

/**
 *
 * @param src
 * @param num
 * @return
 */
point_t **points_new_copy(point_t **const src, size_t num);

/**
 *
 * @param src
 * @param dest
 * @param num
 * @return
 */
point_t **points_clone(point_t **const src, point_t **dest, size_t num);

/**
 *
 * @param src
 * @param num
 * @return
 */
point_t **points_new_clone(point_t **const src, size_t num);

/**
 *
 * @param point
 */
void points_free(point_t ***point);

/**
 *
 * @param points
 * @param npoints
 */
void points_free_deep(point_t ***points, size_t npoints);

/**
 * GENERATOR(gen_t)
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(point_gen_random);

/**
 * GENERATOR(gen_t)
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args size_t number of points to generate
 * @return state diff
 */
GENERATOR(points_gen_random);

/**
 * GENERATOR(gen_t)
 * Generates prime order points using trial division.
 *
 * Assumes the primes divide curve order, thus that points with all
 * prime orders specified exist.
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args pari_ulong array of primes length nargs
 * @return state diff
 */
GENERATOR(points_gen_trial);

/**
 * GENERATOR(gen_t)
 *
 * Cauchy:
 *   Let G be a finite group and p be a prime. If p divides the order of G, then
 *   G has an element of order p.
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(points_gen_prime);

/**
 * GENERATOR(gen_t)
 *
 * Generates points on all subgroups of the curve. Prime and non-prime order.
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(points_gen_allgroups);

/**
 * GENERATOR(gen_t)
 *
 * Generates points on non-prime order of the curve.
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(points_gen_nonprime);

/**
 * UNROLL(unroll_t)
 *
 * @param curve
 * @param cfg
 * @param from
 * @param to
 * @return
 */
UNROLL(points_unroll);

#endif  // ECGEN_POINT_H
