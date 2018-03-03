/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */

#ifndef ECGEN_EXHAUSTIVE_BRAINPOOL_RFC_H
#define ECGEN_EXHAUSTIVE_BRAINPOOL_RFC_H

#include "misc/types.h"

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(brainpool_rfc_gen_seed_argument);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(brainpool_rfc_gen_seed_random);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(brainpool_rfc_gen_seed_input);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(brainpool_rfc_gen_equation);

#endif  // ECGEN_EXHAUSTIVE_BRAINPOOL_RFC_H
