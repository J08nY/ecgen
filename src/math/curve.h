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
GENERATOR(curve_gen_any);

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
GENERATOR(curve_gen_nonzero);

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
GENERATOR(curve_gen_seed);

/**
 *
 * @param curve
 * @param cfg
 * @param from
 * @param to
 * @return
 */
UNROLL(curve_unroll);

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
curve_t *curve_copy(const curve_t *src, curve_t *dest);

/**
 *
 * @param src
 * @return
 */
curve_t *curve_new_copy(const curve_t *src);

/**
 *
 * @param src
 * @param dest
 * @return
 */
curve_t *curve_clone(const curve_t *src, curve_t *dest);

/**
 *
 * @param src
 * @return
 */
curve_t *curve_new_clone(const curve_t *src);

/**
 * Free a curve_t along with it's seed_t and point_ts.
 * @param curve to free
 */
void curve_free(curve_t **curve);

#endif  // ECGEN_CURVE_H
