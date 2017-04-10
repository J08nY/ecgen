/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @brief
 * @file gens.h
 */
#ifndef ECGEN_GENS_H
#define ECGEN_GENS_H

#include "types.h"

/**
 * @brief
 * @param curve
 * @param config
 * @param args
 * @return
 */
int gens_any(curve_t *curve, const config_t *cfg, arg_t *args);

/**
 * @brief
 * @param curve
 * @param config
 * @param args
 * @return
 */
int gens_one(curve_t *curve, const config_t *cfg, arg_t *args);

/**
 *
 * @param curve
 * @param cfg
 * @param from
 * @param to
 * @return
 */
int gens_unroll(curve_t *curve, const config_t *cfg, pari_sp from, pari_sp to);

#endif  // ECGEN_GENS_H
