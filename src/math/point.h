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
 * GENERATOR(gen_t)
 *
 * @param curve
 * @param config
 * @param ... unused
 * @return state diff
 */
int point_random(curve_t *curve, config_t *config, ...);

/**
 * GENERATOR(gen_t)
 *
 * @param curve
 * @param config
 * @param ... size_t number of points to generate
 * @return state diff
 */
int points_random(curve_t *curve, config_t *config, ...);

/**
 * GENERATOR(gen_t)
 *
 * Cauchy:
 *   Let G be a finite group and p be a prime. If p divides the order of G, then
 *   G has an element of order p.
 *
 * @param curve
 * @param config
 * @param ...
 * @return state diff
 */
int points_prime(curve_t *curve, config_t *config, ...);

/**
 * GENERATOR(gen_t)
 * Calculates the minimal set of generators of the curve.(one or two points).
 * Always succeeds.
 *
 * @param curve
 * @param config
 * @param ... unused
 * @return state diff
 */
int points_generators(curve_t *curve, config_t *config, ...);

#endif  // ECGEN_POINT_H
