/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file equation.h
 */
#ifndef ECGEN_EQUATION_H
#define ECGEN_EQUATION_H

#include "io/cli.h"
#include "types.h"

/**
 * GENERATOR(gen_t)
 * Creates a random a parameter by selecting a random field
 * element from the curve field.
 * Always succeeds.
 *
 * @param curve
 * @param config
 * @param ...
 * @return state diff
 */
int a_random(curve_t *curve, config_t *config, ...);

/**
 * GENERATOR(gen_t)
 * Creates a parameter by reading from input.
 *
 * @param curve
 * @param config
 * @param ...
 * @return state diff
 */
int a_input(curve_t *curve, config_t *config, ...);

/**
 * GENERATOR(gen_t)
 * Creates a parameter set to zero.
 *
 * @param curve
 * @param config
 * @param ...
 * @return state diff
 */
int a_zero(curve_t *curve, config_t *config, ...);

/**
 * GENERATOR(gen_t)
 * Creates a parameter set to one.
 *
 * @param curve
 * @param config
 * @param ...
 * @return state diff
 */
int a_one(curve_t *curve, config_t *config, ...);

int a_seed(curve_t *curve, config_t *config, ...);

/**
 * GENERATOR(gen_t)
 * Creates a random b parameter by selecting a random field
 * element from the curve field.
 *
 * @param curve
 * @param config
 * @param ...
 * @return state diff
 */
int b_random(curve_t *curve, config_t *config, ...);

/**
 * GENERATOR(gen_t)
 * Creates b parameter by reading from input.
 *
 * @param curve
 * @param config
 * @param ...
 * @return state diff
 */
int b_input(curve_t *curve, config_t *config, ...);

/**
 * GENERATOR(gen_t)
 * Creates b parameter set to zero.
 *
 * @param curve
 * @param config
 * @param ...
 * @return state diff
 */
int b_zero(curve_t *curve, config_t *config, ...);

/**
 * GENERATOR(gen_t)
 * Creates b parameter set to one.
 *
 * @param curve
 * @param config
 * @param ...
 * @return state diff
 */
int b_one(curve_t *curve, config_t *config, ...);

int b_seed(curve_t *curve, config_t *config, ...);

#endif  // ECGEN_EQUATION_H
