/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file poly.h
 */
#ifndef ECGEN_POLY_H
#define ECGEN_POLY_H

#include <pari/pari.h>
#include <stdbool.h>

typedef struct polynomial_t {
	int m;
	int e1;
	int e2;
	int e3;
} polynomial_t;

/**
 *
 * @param m
 * @return
 */
bool poly_exists(long m);
/**
 *
 * @param m
 * @return
 */
polynomial_t *poly_find(long m);

/**
 *
 * @param m
 * @return
 */
GEN poly_find_gen(long m);

/**
 *
 * @param polynomial
 * @return
 */
GEN poly_gen(polynomial_t *polynomial);

#endif  // ECGEN_POLY_H
