/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_CURVE_H
#define ECGEN_CURVE_H

#include <pari/pari.h>
#include "field.h"
#include "point.h"

typedef struct curve_t {
	GEN seed;
	GEN field;
	GEN a;
	GEN b;
	GEN curve;
	GEN order;
	point_t **points;
	size_t npoints;
} curve_t;

/**
 *
 * @param field
 * @return
 */
curve_t *curve_random(GEN field);

/**
 *
 * @param t
 * @param bits
 * @return
 */
curve_t *curve_randomf(enum field_e t, long bits);

/**
 *
 * @param field
 * @return
 */
curve_t *curve_nonzero(GEN field);

/**
 *
 * @param t
 * @param bits
 * @return
 */
curve_t *curve_nonzerof(enum field_e t, long bits);

/**
 *
 * @param field
 * @return
 */
curve_t *curve_prime(GEN field);

/**
 *
 * @param t
 * @param bits
 * @return
 */
curve_t *curve_primef(enum field_e t, long bits);

/**
 * ANSI X9.62 Verifiable random curve over field with seed.
 *
 * @param field
 * @param seed
 * @return
 */
curve_t *curve_seed(GEN field, GEN seed);

/**
 *
 * @param field
 * @return
 */
curve_t *curve_seedr(GEN field);

/**
 *
 * @param seed
 * @param t
 * @param bits
 * @return
 */
curve_t *curve_seedf(GEN seed, enum field_e t, long bits);

/**
 *
 * @param t
 * @param bits
 * @return
 */
curve_t *curve_seedrf(enum field_e t, long bits);

/**
 *
 * @param curve
 * @return
 */
GEN curve_params(curve_t *curve);

/**
 *
 * @param curve
 */
void free_curve(curve_t **curve);


#endif  // ECGEN_CURVE_H
