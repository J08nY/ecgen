/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file seed.h
 */
#ifndef ECGEN_SEED_H
#define ECGEN_SEED_H

#include "io/input.h"
#include "types.h"

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
seed_t *seed_copy(const seed_t *src, seed_t *dest);

/**
 *
 * @param src
 * @return
 */
seed_t *seed_new_copy(const seed_t *src);

/**
 *
 * @param src
 * @param dest
 * @return
 */
seed_t *seed_clone(const seed_t *src, seed_t *dest);

/**
 *
 * @param src
 * @return
 */
seed_t *seed_new_clone(const seed_t *src);

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
int seed_random(curve_t *curve, const config_t *cfg, arg_t *args);

/**
 *
 * @param curve
 * @param config
 * @param args
 * @return
 */
int seed_argument(curve_t *curve, const config_t *cfg, arg_t *args);

/**
 *
 * @param curve
 * @param config
 * @param args
 * @return
 */
int seed_input(curve_t *curve, const config_t *cfg, arg_t *args);

#endif  // ECGEN_SEED_H
