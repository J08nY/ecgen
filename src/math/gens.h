/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @brief
 * @file gens.h
 */
#ifndef ECGEN_GENS_H
#define ECGEN_GENS_H

#include "types.h"

/**
 * @brief
 * @param curve
 * @param config
 * @param args
 * @return
 */
GENERATOR(gens_gen_any);

/**
 * @brief
 * @param curve
 * @param config
 * @param args
 * @return
 */
GENERATOR(gens_gen_one);

/**
 *
 * @param curve
 * @param cfg
 * @param from
 * @param to
 * @return
 */
UNROLL(gens_unroll);

#endif  // ECGEN_GENS_H
