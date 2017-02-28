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
int gens_any(curve_t *curve, config_t *config, arg_t *args);

/**
 * @brief
 * @param curve
 * @param config
 * @param args
 * @return
 */
int gens_one(curve_t *curve, config_t *config, arg_t *args);

#endif  // ECGEN_GENS_H
