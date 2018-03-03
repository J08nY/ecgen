/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#ifndef ECGEN_MATH_KOBLITZ_H
#define ECGEN_MATH_KOBLITZ_H

#include <stdbool.h>
#include "misc/types.h"

typedef struct {
	unsigned int m;
	unsigned int a;
	const char *hex_order;
} koblitz_t;

/**
 * @brief
 * @param curve
 * @return
 */
bool koblitz_is_curve(const curve_t *curve);

/**
 * @brief
 * @param m
 * @param a
 * @return
 */
const koblitz_t *koblitz_find(unsigned long m, unsigned long a);

/**
 * @brief
 * @param m
 * @param a
 * @return
 */
GEN koblitz_get_order(unsigned long m, unsigned long a);

#endif  // ECGEN_MATH_KOBLITZ_H
