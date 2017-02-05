/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_POLY_H
#define ECGEN_POLY_H

#include <stdbool.h>
#include "gp.h"

typedef struct polynomial {
	int m;
	int e1;
	int e2;
	int e3;
} polynomial_t;

extern polynomial_t ansi_trinomials[];
extern polynomial_t ansi_pentanomials[];

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

#endif //ECGEN_POLY_H
