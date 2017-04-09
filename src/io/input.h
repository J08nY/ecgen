/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file input.h
 */
#ifndef ECGEN_INPUT_H
#define ECGEN_INPUT_H

#include "math/random.h"
#include "math/types.h"

typedef enum PARAM {
	PARAM_PRIME,
	PARAM_INT,
	PARAM_SHORT,
	PARAM_STRING
} param_t;

/**
 *
 * @param prompt
 * @param bits
 * @return
 */
GEN input_prime(const char *prompt, unsigned long bits);

/**
 *
 * @param prompt
 * @param bits
 * @return
 */
GEN input_int(const char *prompt, unsigned long bits);

/**
 *
 * @param prompt
 * @return
 */
GEN input_short(const char *prompt);

/**
 *
 * @param prompt
 * @return
 */
GEN input_string(const char *prompt);

/**
 *
 * @param param
 * @param prompt
 * @param bits
 * @return
 */
GEN input_param(param_t param, const char *prompt, unsigned long bits);

/**
 *
 */
extern FILE *in;

/**
 *
 * @param cfg
 */
void input_init(const config_t *cfg);

/**
 *
 */
void input_quit(void);

#endif  // ECGEN_INPUT_H
