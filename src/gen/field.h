/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file field.h
 */
#ifndef ECGEN_GEN_FIELD_H
#define ECGEN_GEN_FIELD_H

#include "misc/types.h"

/**
 * GENERATOR(gen_f)
 * Creates a random field.
 * Always succeeds.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(field_gen_random);

/**
 * GENERATOR(gen_f)
 * Creates a field by reading:
 *  - a prime number in the prime field case
 *  - three short exponents of the reduction polynomial in the binary case
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(field_gen_input);

/**
 * GENERATOR(gen_f)
 * Creates the field by reading it once.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(field_gen_once);

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
 * Transforms an integer into a field element.
 *
 * @param field the field to work in
 * @param in the integer to transform
 * @return a field element, t_INTMOD or t_FFELT
 */
GEN field_ielement(GEN field, GEN in);

/**
 * @brief
 */
void field_quit(void);

#endif  // ECGEN_GEN_FIELD_H
