
#ifndef ECGEN_ANSI_H
#define ECGEN_ANSI_H

#include "misc/types.h"

bool ansi_seed_valid(const char *hex_str);

/**
 * @brief
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(ansi_gen_seed_random);

/**
 * @brief
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(ansi_gen_seed_argument);

/**
 * @brief
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(ansi_gen_seed_input);

/**
 * @brief
 * @param curve
 * @param cfg
 * @param from
 * @param to
 * @return
 */
UNROLL(ansi_unroll_seed);

/**
 * @brief
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
GENERATOR(ansi_gen_equation);

#endif  // ECGEN_ANSI_H
