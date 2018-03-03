/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file poly.h
 */
#ifndef ECGEN_MATH_POLY_H
#define ECGEN_MATH_POLY_H

#include <pari/pari.h>
#include <stdbool.h>

/**
 * @brief
 */
typedef struct {
	unsigned int m;
	unsigned int e1;
	unsigned int e2;
	unsigned int e3;
} polynomial_t;

/**
 * @brief Whether a polynomial exists in the polynomial data with degree
 * <code>m</code>.
 *
 * @param m the degree of the polynomial searched
 * @return whether it exists
 */
bool poly_exists(unsigned long m);

/**
 * @brief Find a polynomial of degree <code>m</code> in the polynomial dataset.
 *
 * @param m the degree of the polynomial searched
 * @return the polynomial_t * inside the polynomial dataset
 */
const polynomial_t *poly_find(unsigned long m);

/**
 * @brief Turn a polynomial_t into a GEN.
 *
 * @param polynomial the polynomial_t to convert
 * @return a t_POL equal to the polynomial
 */
GEN poly_gen(const polynomial_t *polynomial);

/**
 * @brief Find a polynomial of degree <code>m</code> and turn it into a GEN.
 *
 * @see poly_gen
 * @param m the degree of the polynomial searched
 * @return a t_POL equal to the polynomial
 */
GEN poly_find_gen(unsigned long m);

#endif  // ECGEN_MATH_POLY_H
