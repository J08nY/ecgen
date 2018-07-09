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

/**
 * @brief Construct an elliptic curve given its order, CM discriminant and field
 * order.
 * @param order
 * @param d
 * @param p
 * @param ord_prime
 * @return
 */
GEN cm_construct_curve(GEN order, GEN d, GEN p, bool ord_prime);

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
GENERATOR(cm_gen_order);

#endif  // ECGEN_CM_ANY_H
