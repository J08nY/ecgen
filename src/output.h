/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_OUTPUT_H
#define ECGEN_OUTPUT_H

#include "gp.h"

void output_csv(FILE *out, char delim, char format, GEN vector);

#endif //ECGEN_OUTPUT_H
