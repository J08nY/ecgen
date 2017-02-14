/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_INPUT_H
#define ECGEN_INPUT_H

#include "math/random.h"

typedef enum PARAM {
	PARAM_PRIME,
	PARAM_INT,
	PARAM_SHORT,
	PARAM_STRING
} param_t;

/**
 *
 * @param stream
 * @param prompt
 * @param bits
 * @return
 */
GEN fread_prime(FILE *stream, const char *prompt, long bits);

/**
 *
 * @param stream
 * @param prompt
 * @param bits
 * @return
 */
GEN fread_int(FILE *stream, const char *prompt, long bits);

/**
 *
 * @param stream
 * @param prompt
 * @return
 */
GEN fread_short(FILE *stream, const char *prompt);

GEN fread_string(FILE *stream, const char *prompt);

GEN fread_param(param_t param, FILE *stream, const char *prompt, long bits);

GEN read_param(param_t param, const char *prompt, long bits);

extern FILE *in;

void input_init(const char *input);

void input_quit();

#endif  // ECGEN_INPUT_H
