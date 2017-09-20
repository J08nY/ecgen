/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#ifndef ECGEN_BRAINPOOL_H
#define ECGEN_BRAINPOOL_H

#include "misc/types.h"

GENERATOR(brainpool_gen_seed_random);

GENERATOR(brainpool_gen_seed_argument);

GENERATOR(brainpool_gen_seed_input);

GENERATOR(brainpool_gen_equation);

#endif //ECGEN_BRAINPOOL_H
