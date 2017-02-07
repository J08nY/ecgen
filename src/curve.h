/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_CURVE_H
#define ECGEN_CURVE_H

#include <pari/pari.h>
#include "cli.h"
#include "types.h"

/**
 *
 * @param curve
 * @param config
 * @return
 */
int curve_init(curve_t *curve, config_t *config);

/**
 *
 * @param curve
 * @param config
 * @return
 */
int curve_nonzero(curve_t *curve, config_t *config);

/**
 *
 * @param curve
 * @param config
 * @return
 */
int curve_prime(curve_t *curve, config_t *config);

/**
 *
 * @param curve
 * @param config
 * @return
 */
int curve_seed(curve_t *curve, config_t *config);

/**
 *
 * @param curve
 * @param config
 * @return
 */
int curve_g(curve_t *curve, config_t *config);

/**
 * @param curve
 * @return
 */
GEN curve_params(curve_t *curve);

/**
 *
 * @return
 */
curve_t *curve_new();

/**
 *
 * @param curve
 */
void curve_free(curve_t **curve);

#endif  // ECGEN_CURVE_H
