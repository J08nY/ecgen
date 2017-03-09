/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file curve.h
 */
#ifndef ECGEN_CURVE_H
#define ECGEN_CURVE_H

#include <pari/pari.h>
#include "io/cli.h"
#include "types.h"

/**
 * GENERATOR(gen_t)
 * Creates a curve GEN in curve_t curve from field, a and b.
 * Always succeeds.
 *
 * @param curve
 * @param config
 * @param args unused
 * @return state diff
 */
int curve_any(curve_t *curve, config_t *cfg, arg_t *args);

/**
 * GENERATOR(gen_t)
 * Creates a curve GEN in curve_t curve from field, a and b.
 * Succeeds if a curve exists(non-zero discriminant).
 *
 * @param curve
 * @param config
 * @param args unused
 * @return state diff
 */
int curve_nonzero(curve_t *curve, config_t *cfg, arg_t *args);

/**
 * GENERATOR(gen_t)
 * Creates a curve GEN in curve_t curve from field, a and b. Using the ANSI
 * X9.62 verifiably random algorithm.
 * Succeeds if a curve exists(non-zero discriminant).
 *
 * @param curve
 * @param config
 * @param args unused
 * @return state diff
 */
int curve_seed(curve_t *curve, config_t *cfg, arg_t *args);

/**
 * Serializes curve parameters into a t_VEC:
 * - prime field:
 * 	p,a,b,order,(point.x, point.y, point.order)*
 * - binary field:
 *  e1,e2,e3,a,b,order,(point.x, point.y, point.order)*
 *
 * @param curve to serialize
 * @return a t_VEC of curve parameters
 */
GEN curve_params(curve_t *curve);

/**
 * Allocates and zeros out a new curve_t object.
 * @return new curve
 */
curve_t *curve_new(void);

/**
 * Copies parameters from src curve to dest curve, allocates space for points.
 * Otherwise expects everything to be allocated.
 *
 * @param src source curve
 * @param dest destination curve
 * @return destination curve
 */
curve_t *curve_copy(curve_t *src, curve_t *dest);

/**
 * Free a curve_t along with it's seed_t and point_ts.
 * @param curve to free
 */
void curve_free(curve_t **curve);

#endif  // ECGEN_CURVE_H
