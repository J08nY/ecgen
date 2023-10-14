/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2023 J08nY
 */
/*
 * @file nums.h
 */
#ifndef ECGEN_EXHAUSTIVE_NUMS_H
#define ECGEN_EXHAUSTIVE_NUMS_H

#include "misc/types.h"

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(nums_gen_field);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(nums_gen_a);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(nums_gen_b);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(nums_gen_order);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(nums_gen_gens);

void nums_quit();

#endif  // ECGEN_EXHAUSTIVE_NUMS_H
