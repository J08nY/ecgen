/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#ifndef ECGEN_BRAINPOOL_RFC_H
#define ECGEN_BRAINPOOL_RFC_H

#include "misc/types.h"

/**
 *
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(brainpool_rfc_gen_seed_argument);

/**
 *
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(brainpool_rfc_gen_seed_random);

/**
 *
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

#endif  // ECGEN_BRAINPOOL_RFC_H
