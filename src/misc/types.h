/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file types.h
 */
#ifndef ECGEN_TYPES_H
#define ECGEN_TYPES_H

#include <limits.h>
#include <pari/pari.h>
#include "config.h"

/**
 * @brief
 * @param bits
 * @param bitlen
 * @param allocated
 */
typedef struct {
	unsigned char *bits;
	size_t bitlen;
	size_t allocated;
} bits_t;

/**
 * @brief
 * @param seed
 * @param hash20
 * @param type
 * @param ansi
 * @param brainpool
 */
typedef struct {
	bits_t *seed;
	unsigned char *hash20;
	seed_e type;
	union {
		struct {
			GEN t;
			GEN s;
			GEN h;
			GEN r;
		} ansi;
		struct {
			bits_t *f;
		} brainpool;
	};
} seed_t;

/**
 * @brief A point type.
 * @param point a t_VEC with t_INTMOD or t_FFELT components [x,y]
 * @param order a t_INT
 * @param cofactor a t_INT
 */
typedef struct {
	GEN point;
	GEN order;
	GEN cofactor;
} point_t;

/**
 * @brief A curve type.
 * @param seed a seed_t
 * @param field a t_INT or t_FFELT
 * @param a a t_INTMOD or t_FFELT a parameter
 * @param b a t_INTMOD or t_FFELT b parameter
 * @param curve a t_ELL, curve object
 * @param order a t_INT, curve order
 * @param generators generators saved
 * @param ngens   numver of generators saved in the curve type
 * @param points  points saved
 * @param npoints number of points saved in the curve type
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
	OFFSET_SEED = 0,
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
	void *allocd;
} arg_t;

/**
 * @brief A generator function type.
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args Current optional generator argument
 * @return state diff
 */
#define GENERATOR(gen_name) \
	int gen_name(curve_t *curve, const config_t *cfg, arg_t *args)

typedef GENERATOR((*gen_f));

/**
 * @brief An unroll function type
 * @param curve
 * @param cfg
 * @param from
 * @param to
 * @return
 */
#define UNROLL(unroll_name)                                            \
	int unroll_name(curve_t *curve, const config_t *cfg, pari_sp from, \
	                pari_sp to)

typedef UNROLL((*unroll_f));

/**
 * @brief A check function type.
 * @param curve A curve_t being checked
 * @param cfg An application config
 * @return state diff
 */
#define CHECK(check_name) int check_name(curve_t *curve, const config_t *cfg)

typedef CHECK((*check_f));

/**
 * @brief
 */
typedef struct {
	check_f *checks;
	size_t nchecks;
} check_t;

/**
 * GENERATOR(gen_f)
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(gen_skip);

/**
 * CHECK(check_f)
 *
 * @param curve A curve_t being checked
 * @param cfg An application config
 * @param args unused
 * @return
 */
CHECK(check_skip);

/**
 * UNROLL(unroll_f)
 *
 * @param curve
 * @param cfg
 * @param from
 * @param to
 * @return
 */
UNROLL(unroll_skip);

#endif  // ECGEN_TYPES_H
