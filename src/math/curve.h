/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_CURVE_H
#define ECGEN_CURVE_H

#include <pari/pari.h>
#include "io/cli.h"
#include "types.h"

/**
 * Creates a curve GEN in curve_t curve from field, a and b.
 * Always succeeds.
 *
 * @param curve
 * @param config
 * @return
 */
int curve_init(curve_t *curve, config_t *config, ...);

/**
 * Creates a curve GEN in curve_t curve from field, a and b.
 * Succeeds if a curve exists(non-zero discriminant).
 *
 * @param curve
 * @param config
 * @return
 */
int curve_nonzero(curve_t *curve, config_t *config, ...);

/**
 * Creates a curve GEN in curve_t curve from field, a and b. Using the ANSI
 * X9.62 verifiably random algorithm.
 * Succeeds if a curve exists(non-zero discriminant).
 *
 * @param curve
 * @param config
 * @return
 */
int curve_seed(curve_t *curve, config_t *config, ...);

/**
 * @param curve
 * @return a t_VEC of curve parameters: field,a,b,order
 */
GEN curve_params(curve_t *curve);

/**
 *
 * @return
 */
curve_t *curve_new();

/**
 * Free a curve_t along with it's seed_t and point_ts.
 * @param curve to free
 */
void curve_free(curve_t **curve);

#endif  // ECGEN_CURVE_H
