/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file equation.h
 */
#ifndef ECGEN_GEN_EQUATION_H
#define ECGEN_GEN_EQUATION_H

#include "misc/types.h"

/**
 * GENERATOR(gen_f)
 * Creates a random a parameter by selecting a random field
 * element from the curve field.
 * Always succeeds.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(a_gen_random);

/**
 * GENERATOR(gen_f)
 * Creates a parameter by reading from input.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(a_gen_input);

/**
 * GENERATOR(gen_f)
 * Creates a parameter by reading once from input.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(a_gen_once);

/**
 * GENERATOR(gen_f)
 * Creates a parameter set to zero.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(a_gen_zero);

/**
 * GENERATOR(gen_f)
 * Creates a parameter set to one.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(a_gen_one);

/**
 * GENERATOR(gen_f)
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(a_gen_seed);

/**
 * GENERATOR(gen_f)
 * Creates a random b parameter by selecting a random field
 * element from the curve field.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(b_gen_random);

/**
 * GENERATOR(gen_f)
 * Creates b parameter by reading from input.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(b_gen_input);

/**
 * GENERATOR(gen_f)
 * Creates b parameter by reading once from input.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(b_gen_once);

/**
 * GENERATOR(gen_f)
 * Creates b parameter set to zero.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(b_gen_zero);

/**
 * GENERATOR(gen_f)
 * Creates b parameter set to one.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(b_gen_one);

/**
 * GENERATOR(gen_f)
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(b_gen_seed);

/**
 *
 */
void equation_quit(void);

#endif  // ECGEN_GEN_EQUATION_H
