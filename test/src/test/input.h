/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_TEST_INPUT_H
#define ECGEN_TEST_INPUT_H

#include <stdio.h>

extern FILE *write_in;

void input_setup(void);

void input_teardown(void);

#endif  // ECGEN_TEST_INPUT_H
