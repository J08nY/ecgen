/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#ifndef ECGEN_BRAINPOOL_H
#define ECGEN_BRAINPOOL_H

#include "misc/types.h"

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
GENERATOR(brainpool_gen_equation);

#endif  // ECGEN_BRAINPOOL_H
