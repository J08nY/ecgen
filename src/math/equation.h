/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_EQUATION_H
#define ECGEN_EQUATION_H

#include "io/cli.h"
#include "types.h"

int a_random(curve_t *curve, config_t *config, ...);

int a_input(curve_t *curve, config_t *config, ...);

int a_zero(curve_t *curve, config_t *config, ...);

int a_one(curve_t *curve, config_t *config, ...);

int a_seed(curve_t *curve, config_t *config, ...);

int b_random(curve_t *curve, config_t *config, ...);

int b_input(curve_t *curve, config_t *config, ...);

int b_zero(curve_t *curve, config_t *config, ...);

int b_one(curve_t *curve, config_t *config, ...);

int b_seed(curve_t *curve, config_t *config, ...);

#endif  // ECGEN_EQUATION_H
