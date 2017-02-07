/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_RANDOM_H
#define ECGEN_RANDOM_H

#include <pari/pari.h>
#include <stdbool.h>

bool random_init();

GEN random_prime(long bits);

GEN random_int(long bits);

#endif  // ECGEN_RANDOM_H
