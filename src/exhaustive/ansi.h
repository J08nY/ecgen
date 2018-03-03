/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/*
 * @file ansi.h
 */
#ifndef ECGEN_EXHAUSTIVE_ANSI_H
#define ECGEN_EXHAUSTIVE_ANSI_H

#include "misc/types.h"

bool ansi_seed_valid(const char *hex_str);

/**
 * @brief
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(ansi_gen_seed_random);

/**
 * @brief
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(ansi_gen_seed_argument);

/**
 * @brief
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(ansi_gen_seed_input);

/**
 * @brief
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(ansi_gen_equation);

#endif  // ECGEN_EXHAUSTIVE_ANSI_H
