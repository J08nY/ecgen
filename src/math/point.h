/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file point.h
 */
#ifndef ECGEN_POINT_H
#define ECGEN_POINT_H

#include <pari/pari.h>
#include "types.h"

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
point_t *point_copy(point_t *src, point_t *dest);

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
point_t **points_copy(point_t **src, point_t **dest, size_t num);

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
 * @param curve
 * @param config
 * @param args unused
 * @return state diff
 */
int point_random(curve_t *curve, config_t *config, arg_t *args);

/**
 * GENERATOR(gen_t)
 *
 * @param curve
 * @param config
 * @param args size_t number of points to generate
 * @return state diff
 */
int points_random(curve_t *curve, config_t *config, arg_t *args);

/**
 * GENERATOR(gen_t)
 * Generates prime order points using trial division.
 * The supplied arg is of format:
 *
 * pari_ulong *args->args  primes
 * size_t args->nargs      length of primes
 *
 * Assumes the primes divide curve order, thus that points with all
 * prime orders specified exist.
 *
 * @param curve
 * @param config
 * @param args
 * @return state diff
 */
int points_primet(curve_t *curve, config_t *config, arg_t *args);

/**
 * GENERATOR(gen_t)
 *
 * Cauchy:
 *   Let G be a finite group and p be a prime. If p divides the order of G, then
 *   G has an element of order p.
 *
 * @param curve
 * @param config
 * @param args
 * @return state diff
 */
int points_prime(curve_t *curve, config_t *config, arg_t *args);

#endif  // ECGEN_POINT_H
