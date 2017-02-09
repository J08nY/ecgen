/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_TYPES_H
#define ECGEN_TYPES_H

#include <pari/pari.h>
#include "io/cli.h"

typedef struct seed { GEN seed; } seed_t;

typedef struct point_t {
	GEN point;
	GEN order;
} point_t;

typedef struct curve {
	seed_t *seed;
	GEN field;
	GEN a;
	GEN b;
	GEN curve;
	GEN order;
	point_t **points;
	size_t npoints;
} curve_t;

typedef int (*gen_t)(curve_t *, config_t *, ...);

#endif  // ECGEN_TYPES_H
