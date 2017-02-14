/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_OUTPUT_H
#define ECGEN_OUTPUT_H

#include <pari/pari.h>
#include <stdbool.h>
#include "math/types.h"

/**
 *
 * @param delim
 * @param format
 * @param vector
 * @return
 */
char *output_scsv(const char *format, char delim, GEN vector);

/**
 *
 * @param out
 * @param delim
 * @param format
 * @param vector
 */
void output_csv(FILE *out, const char *format, char delim, GEN vector);

/**
 *
 * @param vector
 * @return
 */
char *output_sjson(curve_t *curve);

/**
 *
 * @param out
 * @param vector
 */
void output_json(FILE *out, GEN vector);

extern FILE *out;
extern FILE *debug;

void output_init(const char *output, bool append);

void output_quit(void);

#endif  // ECGEN_OUTPUT_H
