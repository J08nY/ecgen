/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file subgroups.h
 */
#ifndef ECGEN_SUBGROUPS_H
#define ECGEN_SUBGROUPS_H

#include <pari/pari.h>
#include "gen/types.h"

/**
 * @brief Enumerates prime subgroup orders of a given finite group.
 * @param order group order
 * @param cfg
 * @return
 */
GEN subgroups_prime(GEN i, const config_t *cfg);

/**
 * @brief Enumerates nonprime subgroup orders of a given finite group.
 * @param order group order
 * @param cfg
 * @return
 */
GEN subgroups_nonprime(GEN order, const config_t *cfg);

/**
 * @brief Enumerates all subgroup orders of a given finite group.
 * @param order group order
 * @param cfg
 * @return
 */
GEN subgroups_all(GEN order, const config_t *cfg);


#endif //ECGEN_SUBGROUPS_H
