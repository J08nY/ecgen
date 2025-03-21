/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#ifndef ECGEN_CM_ANY_H
#define ECGEN_CM_ANY_H

#include "misc/types.h"

typedef struct {
	GEN p;
	GEN d;
} cm_any_qdisc_t;

typedef struct {
	GEN roots;
	long used;
	long total;
} cm_any_roots_t;

/**
 *
 * @param d
 * @param p
 * @param roots
 */
void cm_update_roots(GEN d, GEN p, cm_any_roots_t *roots);

/**
 *
 * @param d
 * @param p
 * @return
 */
cm_any_roots_t *cm_make_roots(GEN d, GEN p);

void cm_free_roots(cm_any_roots_t *roots);

/**
 * @brief Construct an elliptic curve given its order, CM discriminant and field
 * order.
 * @param order
 * @param d
 * @param p
 * @param roots
 * @param ord_prime
 * @return
 */
GEN cm_construct_curve(GEN order, GEN d, GEN p, cm_any_roots_t *roots,
                       bool ord_prime);

/**
 * @brief Construct an elliptic curve given a factor of its order, CM
 * discriminant and field order.
 *
 * @param r
 * @param d
 * @param p
 * @param roots
 */
GEN cm_construct_curve_subgroup(GEN r, GEN d, GEN p, cm_any_roots_t *roots);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(cm_gen_curve_any);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(cm_gen_curve_unique);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(cm_gen_order);

/**
 * @brief Deinitialize.
 */
void cm_any_quit();

#endif  // ECGEN_CM_ANY_H
