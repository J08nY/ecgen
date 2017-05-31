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
 *
 * @return
 */
seed_t *seed_new(void);

/**
 *
 * @param src
 * @param dest
 * @return
 */
seed_t *seed_copy(const seed_t *src, seed_t *dest);

/**
 *
 * @param src
 * @return
 */
seed_t *seed_new_copy(const seed_t *src);

/**
 *
 * @param src
 * @param dest
 * @return
 */
seed_t *seed_clone(const seed_t *src, seed_t *dest);

/**
 *
 * @param src
 * @return
 */
seed_t *seed_new_clone(const seed_t *src);

/**
 *
 * @param seed
 */
void seed_free(seed_t **seed);

/**
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(seed_gen_random);

/**
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(seed_gen_argument);

/**
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(seed_gen_input);

#endif  // ECGEN_SEED_H
