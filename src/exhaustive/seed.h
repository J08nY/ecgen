/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file seed.h
 */
#ifndef ECGEN_SEED_H
#define ECGEN_SEED_H

#include "io/cli.h"
#include "math/types.h"

/**
 *
 * @return
 */
seed_t *seed_new(void);

/**
 *
 * @param src
 * @param dest
 * @return
 */
seed_t *seed_copy(seed_t *src, seed_t *dest);

/**
 *
 * @param seed
 */
void seed_free(seed_t **seed);

/**
 *
 * @param curve
 * @param config
 * @param args
 * @return
 */
int seed_random(curve_t *curve, config_t *config, arg_t *args);

/**
 *
 * @param curve
 * @param config
 * @param args
 * @return
 */
int seed_argument(curve_t *curve, config_t *config, arg_t *args);

/**
 *
 * @param curve
 * @param config
 * @param args
 * @return
 */
int seed_input(curve_t *curve, config_t *config, arg_t *args);

#endif  // ECGEN_SEED_H
