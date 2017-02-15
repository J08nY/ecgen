/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file types.h
 */
#ifndef ECGEN_TYPES_H
#define ECGEN_TYPES_H

#include <pari/pari.h>
#include "io/cli.h"

typedef struct seed_t { GEN seed; } seed_t;

typedef struct point_t {
	GEN point;
	GEN order;
} point_t;

typedef struct curve_t {
	seed_t *seed;
	GEN field;
	GEN a;
	GEN b;
	GEN curve;
	GEN order;
	point_t **points;
	size_t npoints;
} curve_t;

enum curve_offset {
	OFFSET_SEED,
	OFFSET_FIELD,
	OFFSET_A,
	OFFSET_B,
	OFFSET_CURVE,
	OFFSET_ORDER,
	OFFSET_POINTS,
	OFFSET_END
};

typedef int (*gen_t)(curve_t *, config_t *, ...);

int gen_skip(curve_t *curve, config_t *config, ...);

#endif  // ECGEN_TYPES_H
