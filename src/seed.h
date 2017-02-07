/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_SEED_H
#define ECGEN_SEED_H

#include "cli.h"
#include "types.h"

void seed_free(seed_t **seed);

int seed_random(curve_t *curve, config_t *config);

int seed_argument(curve_t *curve, config_t *config);

int seed_input(curve_t *curve, config_t *config);

int seed_g(curve_t *curve, config_t *config);

#endif  // ECGEN_SEED_H
