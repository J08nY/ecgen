/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file subgroup.h
 */
#ifndef ECGEN_MATH_SUBGROUPS_H
#define ECGEN_MATH_SUBGROUPS_H

#include <pari/pari.h>
#include "misc/types.h"

/**
 * @brief All prime factors of a given integer, without multipliticity.
 *
 * subgroups_factors(27) = [3]
 * @param order
 * @return a t_VEC of prime factors.
 */
GEN subgroups_prime(GEN order);

/**
 * @brief All nonprime subgroup orders of a given integer.
 * @param order
 * @return a t_VEC of nonprime factors.
 */
GEN subgroups_nonprime(GEN order);

/**
 * @brief All all subgroup orders of a given integer.
 * @param order
 * @return a t_VEC of all factors.
 */
GEN subgroups_all(GEN order);

#endif  // ECGEN_MATH_SUBGROUPS_H
