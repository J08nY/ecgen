/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file equation.h
 */
#ifndef ECGEN_EQUATION_H
#define ECGEN_EQUATION_H

#include "misc/types.h"

/**
 * GENERATOR(gen_t)
 * Creates a random a parameter by selecting a random field
 * element from the curve field.
 * Always succeeds.
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(a_gen_random);

/**
 * GENERATOR(gen_t)
 * Creates a parameter by reading from input.
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(a_gen_input);

/**
 * GENERATOR(gen_t)
 * Creates a parameter by reading once from input.
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(a_gen_once);

/**
 * GENERATOR(gen_t)
 * Creates a parameter set to zero.
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(a_gen_zero);

/**
 * GENERATOR(gen_t)
 * Creates a parameter set to one.
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(a_gen_one);

/**
 * GENERATOR(gen_t)
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(a_gen_seed);

/**
 * GENERATOR(gen_t)
 * Creates a random b parameter by selecting a random field
 * element from the curve field.
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(b_gen_random);

/**
 * GENERATOR(gen_t)
 * Creates b parameter by reading from input.
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(b_gen_input);

/**
 * GENERATOR(gen_t)
 * Creates b parameter by reading once from input.
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(b_gen_once);

/**
 * GENERATOR(gen_t)
 * Creates b parameter set to zero.
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(b_gen_zero);

/**
 * GENERATOR(gen_t)
 * Creates b parameter set to one.
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(b_gen_one);

/**
 * GENERATOR(gen_t)
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(b_gen_seed);

/**
 *
 */
void equation_quit(void);

#endif  // ECGEN_EQUATION_H
