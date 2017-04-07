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
#include "io/config.h"

typedef struct { GEN seed; } seed_t;

typedef struct {
	GEN point;
	GEN order;
	GEN cofactor;
} point_t;

typedef struct {
	seed_t *seed;
	GEN field;
	GEN a;
	GEN b;
	GEN curve;
	GEN order;
	point_t **generators;
	size_t ngens;
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
	OFFSET_GENERATORS,
	OFFSET_POINTS,
	OFFSET_END
};

typedef struct {
	void *args;
	size_t nargs;
} arg_t;

typedef int (*gen_t)(curve_t *, config_t *, arg_t *);

/**
 * @brief
 * @param curve
 * @param config
 * @param args
 * @return
 */
int gen_skip(curve_t *curve, config_t *cfg, arg_t *args);

#endif  // ECGEN_TYPES_H
