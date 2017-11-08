/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include "brainpool.h"

GENERATOR(brainpool_gen_seed_random) { return INT_MIN; }

GENERATOR(brainpool_gen_seed_argument) { return INT_MIN; }

GENERATOR(brainpool_gen_seed_input) { return INT_MIN; }

GENERATOR(brainpool_gen_equation) { return INT_MIN; }