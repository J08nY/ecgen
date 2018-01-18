/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#ifndef ECGEN_TWIST_H
#define ECGEN_TWIST_H

#include "misc/types.h"

/**
 * @brief Twists the <code>of</code> curve randomly.
 * @param to The result of the twist.
 * @param of The curve to be twisted.
 */
void twist_rand_to(curve_t *to, const curve_t *of);

/**
 * @brief
 * @param what
 */
void twist_rand(curve_t *what);

#endif  // ECGEN_TWIST_H
