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

typedef struct {
	unsigned int m;
	unsigned int e1;
	unsigned int e2;
	unsigned int e3;
} polynomial_t;

/**
 *
 * @param m
 * @return
 */
bool poly_exists(unsigned long m);
/**
 *
 * @param m
 * @return
 */
polynomial_t *poly_find(unsigned long m);

/**
 *
 * @param m
 * @return
 */
GEN poly_find_gen(unsigned long m);

/**
 *
 * @param polynomial
 * @return
 */
GEN poly_gen(const polynomial_t *polynomial);

#endif  // ECGEN_POLY_H
