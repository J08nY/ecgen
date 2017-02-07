/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_FIELD_H
#define ECGEN_FIELD_H

#include "cli.h"
#include "types.h"

/**
 *
 * @param curve
 * @param config
 * @return
 */
int field_random(curve_t *curve, config_t *config);

/**
 *
 * @param curve
 * @param config
 * @return
 */
int field_input(curve_t *curve, config_t *config);

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
