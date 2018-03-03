/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */

#ifndef ECGEN_EXHAUSTIVE_BRAINPOOL_H
#define ECGEN_EXHAUSTIVE_BRAINPOOL_H

#include "misc/types.h"

/**
 *
 * @param s
 */
void brainpool_update_seed(bits_t *s);

/**
 *
 * @param s
 * @param w
 * @param v
 * @return
 */
bits_t *brainpool_hash(const bits_t *s, long w, long v);

/**
 * @brief
 * @param hex_str
 * @return
 */
bool brainpool_seed_valid(const char *hex_str);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(brainpool_gen_seed_random);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(brainpool_gen_seed_argument);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(brainpool_gen_seed_input);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(brainpool_gen_field);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(brainpool_gen_equation);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(brainpool_gen_gens);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
CHECK(brainpool_check_gens);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
CHECK(brainpool_check_order);

#endif  // ECGEN_EXHAUSTIVE_BRAINPOOL_H
