/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#ifndef ECGEN_MATH_TWIST_H
#define ECGEN_MATH_TWIST_H

#include "misc/types.h"

/**
 * @brief Twists the <code>of</code> curve randomly.
 * @param to The result of the twist.
 * @param of The curve to be twisted.
 */
void twist_rand_to(curve_t *to, const curve_t *of);

/**
 * @brief Twists the <code>what</code> curve randomly, inplae.
 * @param what The curve to be twisted, also the result.
 */
void twist_rand(curve_t *what);

#endif  // ECGEN_MATH_TWIST_H
