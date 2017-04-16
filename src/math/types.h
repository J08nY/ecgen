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

/**
 * @brief
 */
typedef struct seed_t {
	char *raw;
	size_t raw_len;
	GEN seed;
} seed_t;

/**
 * @brief
 */
typedef struct {
	GEN point;
	GEN order;
	GEN cofactor;
} point_t;

/**
 * @brief
 */
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

/**
 * @brief
 */
typedef enum {
	OFFSET_SEED,
	OFFSET_FIELD,
	OFFSET_A,
	OFFSET_B,
	OFFSET_CURVE,
	OFFSET_ORDER,
	OFFSET_GENERATORS,
	OFFSET_POINTS,
	OFFSET_END
} offset_e;

/**
 * @brief
 */
typedef struct {
	const void *args;
	size_t nargs;
} arg_t;

/**
 * @brief
 */
typedef int (*gen_t)(curve_t *, const config_t *, arg_t *);

/**
 * @brief
 */
typedef int (*unroll_t)(curve_t *, const config_t *, pari_sp, pari_sp);

/**
 *
 * @param curve
 * @param config
 * @param args
 * @return
 */
int gen_skip(curve_t *curve, const config_t *cfg, arg_t *args);

/**
 *
 * @param curve
 * @param cfg
 * @param from
 * @param to
 * @return
 */
int unroll_skip(curve_t *curve, const config_t *cfg, pari_sp from, pari_sp to);

#endif  // ECGEN_TYPES_H
