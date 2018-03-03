/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @brief
 * @file gens.h
 */
#ifndef ECGEN_GEN_GENS_H
#define ECGEN_GEN_GENS_H

#include "misc/types.h"

/**
 * GENERATOR(gen_f)
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @param state
 * @return state diff
 */
GENERATOR(gens_gen_any);

/**
 * GENERATOR(gen_f)
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @param state
 * @return state diff
 */
GENERATOR(gens_gen_one);

/**
 * GENERATOR(gen_f)
 *
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(gens_gen_cofactor);

/**
 * CHECK(check_f)
 *
 * @param curve
 * @param args
 * @param state
 * @return
 */
CHECK(gens_check_anomalous);

/**
 * @brief Get the embedding degree of a subgroup of <code>order</code> in a
 * power of F_prime.
 *
 * @param prime The order of the base field.
 * @param order The order of the subgroup generator (in the curve group).
 * @return The embedding degree 't' such that <code>order</code> divides
 * 'prime^t - 1'.
 */
GEN gens_get_embedding(GEN prime, GEN order);

/**
 * CHECK(check_f)
 *
 * @param curve
 * @param args
 * @param state
 * @return
 */
CHECK(gens_check_embedding);

/**
 * UNROLL(unroll_f)
 *
 * @param curve
 * @param from
 * @param to
 * @return
 */
UNROLL(gens_unroll);

#endif  // ECGEN_GEN_GENS_H
