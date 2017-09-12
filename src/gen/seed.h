/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file seed.h
 */
#ifndef ECGEN_SEED_H
#define ECGEN_SEED_H

#include "io/input.h"
#include "types.h"

/**
 * @brief
 * @return
 */
seed_t *seed_new(void);

/**
 * @brief
 * @param src
 * @param dest
 * @return
 */
seed_t *seed_copy(const seed_t *src, seed_t *dest);

/**
 * @brief
 * @param src
 * @return
 */
seed_t *seed_new_copy(const seed_t *src);

/**
 * @brief
 * @param src
 * @param dest
 * @return
 */
seed_t *seed_clone(const seed_t *src, seed_t *dest);

/**
 * @brief
 * @param src
 * @return
 */
seed_t *seed_new_clone(const seed_t *src);

/**
 * @brief
 * @param seed
 */
void seed_free(seed_t **seed);

/**
 * @brief
 * @param hex_str
 * @return
 */
bool seed_valid(const char *hex_str);

/**
 * @brief
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(seed_gen_random);

/**
 * @brief
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(seed_gen_argument);

/**
 * @brief
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(seed_gen_input);

#endif  // ECGEN_SEED_H
