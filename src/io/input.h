/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file input.h
 */
#ifndef ECGEN_IO_INPUT_H
#define ECGEN_IO_INPUT_H

#include "misc/types.h"
#include "util/random.h"

/**
 * @brief Input a prime, upto <code>bits</code> long.
 * @param prompt
 * @param bits
 * @return the prime as t_INT, or <code>-1</code> if a composite was given.
 */
GEN input_prime(const char *prompt, unsigned long bits);

/**
 * @brief Input an unsigned integer, upto <code>bits</code> long.
 * @param prompt
 * @param bits
 * @return the int as t_INT, or <code>-1</code> if invalid input was given.
 */
GEN input_int(const char *prompt, unsigned long bits);

/**
 * @brief Input a short unsigned integer. (16 bits wide).
 * @param prompt
 * @return the short as t_INT, or <code>-1</code> if invalid input was given.
 */
GEN input_short(const char *prompt);

/**
 * @brief Input a string.
 * @param prompt
 * @return the string as a t_STR
 */
GEN input_string(const char *prompt);

/**
 * @brief The input FILE * to read all input from, can be <code>stdin</code>.
 */
extern FILE *in;

/**
 * @brief Initialize input based on cfg.
 * @return whether the initialization was successful
 */
bool input_init();

/**
 * @brief Deinitialize input.
 */
void input_quit(void);

#endif  // ECGEN_IO_INPUT_H
