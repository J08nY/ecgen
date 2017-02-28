/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file order.h
 */
#ifndef ECGEN_ORDER_H
#define ECGEN_ORDER_H

#include "types.h"

/**
 * GENERATOR(gen_t)
 * Calculates the curve order, using a general algorithm.
 * Always succeeds.
 *
 * @param curve
 * @param cfg
 * @param args
 * @return state diff
 */
int order_any(curve_t *curve, config_t *cfg, arg_t *args);

/**
 * GENERATOR(gen_t)
 *
 * @param curve
 * @param cfg
 * @param args
 * @return
 */
int order_smallfact(curve_t *curve, config_t *cfg, arg_t *args);

/**
 * GENERATOR(gen_t)
 * Calculates the curve order, always using the SEA algorithm,
 * gives up early in case the order is divisible by "something".
 * Succeeds if the curve has a prime order.
 *
 * @param curve
 * @param cfg
 * @param args
 * @return state diff
 */
int order_prime(curve_t *curve, config_t *cfg, arg_t *args);

#endif  // ECGEN_ORDER_H
