/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#ifndef ECGEN_KOBLITZ_H
#define ECGEN_KOBLITZ_H

#include <stdbool.h>
#include "misc/types.h"

typedef struct {
	unsigned int m;
	unsigned int a;
	const char *hex_order;
} koblitz_t;

bool koblitz_is_curve(const curve_t *curve);

const koblitz_t *koblitz_find(unsigned int m, unsigned int a);

GEN koblitz_get_order(unsigned long m, unsigned int a);

#endif  // ECGEN_KOBLITZ_H
