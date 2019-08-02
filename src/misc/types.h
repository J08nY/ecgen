/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file types.h
 */
#ifndef ECGEN_MISC_TYPES_H
#define ECGEN_MISC_TYPES_H

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
			bool update_seed;
			long w;
			long v;
			bits_t *seed_a;
			bits_t *seed_b;
			GEN mult;
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
 * @brief A subgroup type.
 * @param generator a point generating the subgroup
 * @param npoints number of points stored in the subgroup
 * @param points the stored points
 */
typedef struct {
	point_t *generator;
	size_t npoints;
	point_t **points;
} subgroup_t;

/**
 * @brief Curve metadata.
 */
typedef struct {
	GEN j_invariant;
	GEN discriminant;
	GEN cm_discriminant;
	GEN frobenius_trace;
	GEN embedding_degree;
	GEN conductor;
} metadata_t;

/**
 * @brief A curve type.
 * @param seed a seed_t
 * @param field a t_INT or t_FFELT
 * @param a a t_INTMOD or t_FFELT a parameter
 * @param b a t_INTMOD or t_FFELT b parameter
 * @param curve a t_ELL, curve object
 * @param order a t_INT, curve order
 * @param generators generators saved
 * @param meta
 * @param ngens number of generators saved in the curve type
 */
typedef struct {
	seed_t *seed;
	GEN field;
	GEN a;
	GEN b;
	GEN curve;
	GEN order;
	subgroup_t **generators;
	size_t ngens;
	metadata_t meta;
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
	OFFSET_METADATA,
	OFFSET_END
} offset_e;

extern const char *offset_s[OFFSET_END];

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
 * @param args Current optional generator argument
 * @param state The current generation state
 * @return state diff
 */
#define GENERATOR(gen_name) \
	int gen_name(curve_t *curve, arg_t *args, offset_e state)

typedef GENERATOR((*gen_f));

/**
 * @brief An unroll function type
 * @param curve
 * @param from
 * @param to
 * @return
 */
#define UNROLL(unroll_name) \
	int unroll_name(curve_t *curve, pari_sp from, pari_sp to)

typedef UNROLL((*unroll_f));

/**
 * @brief A check function type.
 * @param curve A curve_t being checked
 * @param args Current optional generator argument
 * @return state diff
 */
#define CHECK(check_name) GENERATOR(check_name)

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
 * @param args unused
 * @return state diff
 */
GENERATOR(gen_skip);

/**
 * CHECK(check_f)
 *
 * @param curve A curve_t being checked
 * @param args unused
 * @return
 */
CHECK(check_skip);

/**
 * UNROLL(unroll_f)
 *
 * @param curve
 * @param from
 * @param to
 * @return
 */
UNROLL(unroll_skip);

#endif  // ECGEN_MISC_TYPES_H
