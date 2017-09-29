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

#include "misc/types.h"

/**
 * GENERATOR(gen_f)
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(gens_gen_any);

/**
 * GENERATOR(gen_f)
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(gens_gen_one);

/**
 * UNROLL(unroll_f)
 *
 * @param curve
 * @param cfg
 * @param from
 * @param to
 * @return
 */
UNROLL(gens_unroll);

#endif  // ECGEN_GENS_H
