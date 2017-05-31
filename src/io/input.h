/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file input.h
 */
#ifndef ECGEN_INPUT_H
#define ECGEN_INPUT_H

#include "util/random.h"
#include "gen/types.h"

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
 */
extern FILE *in;

/**
 * @brief Initialize input based on cfg.
 * @param cfg
 * @return whether the initialization was successful
 */
bool input_init(const config_t *cfg);

/**
 * @brief Deinitialize input.
 */
void input_quit(void);

#endif  // ECGEN_INPUT_H
