/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_TEST_OUTPUT_H
#define ECGEN_TEST_OUTPUT_H

#include <stdio.h>

extern FILE *read_out;
extern FILE *read_err;
extern FILE *read_verbose;

void output_setup(void);

void output_teardown(void);

#endif  // ECGEN_TEST_OUTPUT_H
