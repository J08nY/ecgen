/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file subgroups.h
 */
#ifndef ECGEN_SUBGROUPS_H
#define ECGEN_SUBGROUPS_H

#include <pari/pari.h>
#include "misc/types.h"

/**
 * @brief Enumerates prime subgroup orders of a given curve.
 * @param curve
 * @return
 */
GEN subgroups_prime(const curve_t *curve);

/**
 * @brief Enumerates nonprime subgroup orders of a given curve.
 * @param curve
 * @return
 */
GEN subgroups_nonprime(const curve_t *curve);

/**
 * @brief Enumerates all subgroup orders of a given curve.
 * @param curve
 * @return
 */
GEN subgroups_all(const curve_t *curve);

#endif  // ECGEN_SUBGROUPS_H
