/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file output.h
 */
#ifndef ECGEN_OUTPUT_H
#define ECGEN_OUTPUT_H

#include <pari/pari.h>
#include <stdbool.h>
#include "math/types.h"

/**
 *
 * @param curve
 * @param config
 * @return
 */
char *output_scsv(curve_t *curve, const config_t *cfg);

/**
 *
 * @param out
 * @param curve
 * @param config
 */
void output_fcsv(FILE *out, curve_t *curve, const config_t *cfg);

/**
 *
 * @param curve
 * @param config
 */
void output_csv(curve_t *curve, const config_t *cfg);

/**
 *
 * @param curve
 * @param config
 * @return
 */
char *output_sjson(curve_t *curve, const config_t *cfg);

/**
 *
 * @param out
 * @param curve
 * @param config
 */
void output_fjson(FILE *out, curve_t *curve, const config_t *cfg);

/**
 *
 * @param curve
 * @param config
 */
void output_json(curve_t *curve, const config_t *cfg);

/**
 *
 * @param curve
 * @param config
 * @return
 */
char *(*output_s)(curve_t *curve, const config_t *cfg);

/**
 *
 * @param out
 * @param curve
 * @param config
 */
void (*output_f)(FILE *out, curve_t *curve, const config_t *cfg);

/**
 *
 * @param curve
 * @param config
 */
void (*output_o)(curve_t *curve, const config_t *cfg);

/**
 *
 */
extern FILE *out;

/**
 *
 */
extern FILE *verbose;

/**
 *
 * @param cfg
 */
void output_init(const config_t *cfg);

/**
 *
 */
void output_quit(void);

#ifdef DEBUG
#define debug(...) fprintf(out, __VA_ARGS__)
#else
#define debug(...)
#endif

#endif  // ECGEN_OUTPUT_H
