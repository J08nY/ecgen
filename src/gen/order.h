/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file order.h
 */
#ifndef ECGEN_ORDER_H
#define ECGEN_ORDER_H

#include "misc/types.h"

/**
 * GENERATOR(gen_f)
 * Reads the curve order from input, does not verify it.
 *
 * @param curve A curve_t being generated
 * @param args Current optional generator argument
 * @param state
 * @return state diff
 */
GENERATOR(order_gen_input);

/**
 * GENERATOR(gen_f)
 * Calculates the curve order, using a general algorithm.
 * Always succeeds.
 *
 * @param curve A curve_t being generated
 * @param args Current optional generator argument
 * @param state
 * @return state diff
 */
GENERATOR(order_gen_any);

/**
 * GENERATOR(gen_f)
 * Calculates the curve order, using the SEA algorithm.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @param state
 * @return state diff
 */
GENERATOR(order_gen_sea);

/**
 * GENERATOR(gen_f)
 *
 * @param curve A curve_t being generated
 * @param args pari_ulong passed to ellsea(curve, smallfact)
 * @param state
 * @return state diff
 */
GENERATOR(order_gen_smallfact);

/**
 * GENERATOR(gen_f)
 * Calculates the curve order, always using the SEA algorithm,
 * gives up early in case the order is divisible by "something".
 * Succeeds if the curve has a prime order.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @param state
 * @return state diff
 */
GENERATOR(order_gen_prime);

/**
 *
 * @param curve
 * @param args
 * @param state
 * @return
 */
CHECK(order_check_pohlig_hellman);

/**
 *
 * @param curve
 * @param args
 * @param state
 * @return
 */
CHECK(order_check_discriminant);

#endif  // ECGEN_ORDER_H
