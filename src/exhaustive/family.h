/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2024 J08nY
 */
/*
 * @file family.h
 */
#ifndef ECGEN_EXHAUSTIVE_FAMILY_H
#define ECGEN_EXHAUSTIVE_FAMILY_H

#include "misc/types.h"

GENERATOR(family_gen_seed_random);

GENERATOR(family_gen_seed_input);

GENERATOR(family_gen_field);

GENERATOR(family_gen_equation_iter);

GENERATOR(family_gen_equation_cm);

GENERATOR(family_gen_order);

void family_init();

void family_quit();

#endif  // ECGEN_EXHAUSTIVE_FAMILY_H
