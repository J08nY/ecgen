/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_FIELD_H
#define ECGEN_FIELD_H

#include "poly.h"
#include "random.h"
#include "cli.h"

/**
 *
 * @param bits
 * @return
 */
GEN field_primer(long bits);

/**
 *
 * @param bits
 * @return
 */
GEN field_binaryr(long bits);

/**
 *
 * @param t
 * @param bits
 * @return
 */
GEN field_random(enum field_e t, long bits);

/**
 * Extract a field representation from a field.
 *  - char(field) == 2:
 *    returns the vector of powers of middle coefficients of the reduction
 * polynomial.
 *  - char(field) != 2:
 *     returns the field characteristic(p).-
 *
 * @param field
 * @return field representation
 */
GEN field_params(GEN field);

/**
 *
 * @param element
 * @return
 */
GEN field_elementi(GEN element);

#endif  // ECGEN_FIELD_H
