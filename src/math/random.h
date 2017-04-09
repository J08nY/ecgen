/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file random.h
 */
#ifndef ECGEN_RANDOM_H
#define ECGEN_RANDOM_H

#include <pari/pari.h>
#include <stdbool.h>

bool random_init(void);

GEN random_prime(unsigned long bits);

GEN random_int(unsigned long bits);

#endif  // ECGEN_RANDOM_H
