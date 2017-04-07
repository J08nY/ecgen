/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file field.h
 */
#ifndef ECGEN_FIELD_H
#define ECGEN_FIELD_H

#include "io/cli.h"
#include "types.h"

/**
 * GENERATOR(gen_t)
 * Creates a random field.
 * Always succeeds.
 *
 * @param curve
 * @param config
 * @param args unused
 * @return state diff
 */
int field_random(curve_t *curve, config_t *cfg, arg_t *args);

/**
 * GENERATOR(gen_t)
 * Creates a field by reading:
 *  - a prime number in the prime field case
 *  - three short exponents of the reduction polynomial in the binary case
 *
 * @param curve
 * @param config
 * @param args unused
 * @return state diff
 */
int field_input(curve_t *curve, config_t *cfg, arg_t *args);

/**
 *
 * @param curve
 * @param cfg
 * @param args
 * @return
 */
int field_once(curve_t *curve, config_t *cfg, arg_t *args);

/**
 * Extract a field representation from a field.
 *  - char(field) == 2:
 *    returns the vector of powers of middle coefficients of the reduction
 * polynomial.
 *  - char(field) != 2:
 *     returns the vector of the field characteristic(p).
 *
 * @param field
 * @return field representation
 */
GEN field_params(GEN field);

/**
 * Transforms a field element to an integer.
 * Uses the polynomial basis of the underlying field in case of a binary field.
 *
 * @param element t_INTMOD, t_INT, t_FFELT to transform
 * @return t_INT
 */
GEN field_elementi(GEN element);

/**
 *
 * @param field
 * @param in
 * @return
 */
GEN field_ielement(GEN field, GEN in);

#endif  // ECGEN_FIELD_H
