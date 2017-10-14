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
#include "misc/types.h"

#ifdef DEBUG

#include <inttypes.h>
#include <time.h>

#define _debug_print(prefix)             \
	fprintf(verbose, prefix);            \
	struct timespec ts;                  \
	clock_gettime(CLOCK_MONOTONIC, &ts); \
	fprintf(verbose, "%" PRIdMAX ".%.9ld ", ts.tv_sec, ts.tv_nsec);

#define debug(...) pari_fprintf(verbose, __VA_ARGS__)
#define debug_log(...)          \
	do {                        \
		_debug_print(" -  ");   \
		debug(__VA_ARGS__);     \
		fprintf(verbose, "\n"); \
	} while (0)

#define debug_log_start(...)    \
	do {                        \
		_debug_print("[ ] ");   \
		debug(__VA_ARGS__);     \
		fprintf(verbose, "\n"); \
	} while (0)

#define debug_log_end(...)      \
	do {                        \
		_debug_print("[*] ");   \
		debug(__VA_ARGS__);     \
		fprintf(verbose, "\n"); \
	} while (0)

#else
#define debug(...)
#define debug_log(...)
#define debug_log_start(...)
#define debug_log_end(...)
#endif  // DEBUG

#define verbose_log(...) \
	if (cfg->verbose) fprintf(verbose, __VA_ARGS__)

#define output_log(...) fprintf(out, __VA_ARGS__)

/**
 * @brief Output curve to a malloc'ed string in CSV format.
 * @param curve
 * @param config
 * @return
 */
char *output_scsv(curve_t *curve, const config_t *cfg);

/**
 * @brief Output CSV separator(newline) to a malloc'ed string in CSV
 * format.
 * @param cfg
 * @return
 */
char *output_scsv_separator(const config_t *cfg);

/**
 * @brief Output CSV output header to a malloc'ed string in CSV format.
 * @param cfg
 * @return
 */
char *output_scsv_begin(const config_t *cfg);

/**
 * @brief Output CSV output footer to a malloc'ed string in CSV format.
 * @param cfg
 * @return
 */
char *output_scsv_end(const config_t *cfg);

/**
 * @brief Output curve to a malloc'ed string in JSON format.
 * @param curve
 * @param config
 * @return
 */
char *output_sjson(curve_t *curve, const config_t *cfg);

/**
 * @brief Output JSON separator(a ",\n") to a malloc'ed string in CSV
 * format.
 * @param cfg
 * @return
 */
char *output_sjson_separator(const config_t *cfg);

/**
 * @brief Output JSON output header(a "[") to a malloc'ed string in CSV
 * format.
 * @param cfg
 * @return
 */
char *output_sjson_begin(const config_t *cfg);

/**
 * @brief Output JSON output footer(a "]") to a malloc'ed string in CSV
 * format.
 * @param cfg
 * @return
 */
char *output_sjson_end(const config_t *cfg);

/**
 * @brief Output curve to a malloc'ed string in configured format.
 * @param curve
 * @param config
 * @return
 */
char *(*output_s)(curve_t *curve, const config_t *cfg);

/**
 * @brief Output curve to a FILE *out in configured format.
 * @param out
 * @param curve
 * @param config
 */
void output_f(FILE *out, curve_t *curve, const config_t *cfg);

/**
 * @brief Output curve to configured output in configured format.
 * @param curve
 * @param config
 */
void output_o(curve_t *curve, const config_t *cfg);

/**
 * @brief Output separator to a malloc'ed string in configured format.
 * @param cfg
 * @return
 */
char *(*output_s_separator)(const config_t *cfg);

/**
 * @brief Output separator to a FILE *out in configured format.
 * @param out
 * @param cfg
 */
void output_f_separator(FILE *out, const config_t *cfg);

/**
 * @brief Output separator to configured output in configured format.
 * @param cfg
 */
void output_o_separator(const config_t *cfg);

/**
 * @brief Output header to a malloc'ed string in configured format.
 * @param cfg
 * @return
 */
char *(*output_s_begin)(const config_t *cfg);

/**
 * @brief Output header to a FILE *out in configured format.
 * @param out
 * @param cfg
 */
void output_f_begin(FILE *out, const config_t *cfg);

/**
 * @brief Output header to configured output in configured format.
 * @param cfg
 */
void output_o_begin(const config_t *cfg);

/**
 * @brief Output footer to a malloc'ed string in configured format.
 * @param cfg
 * @return
 */
char *(*output_s_end)(const config_t *cfg);

/**
 * @brief Output footer to a FILE *out in configured format.
 * @param out
 * @param cfg
 */
void output_f_end(FILE *out, const config_t *cfg);

/**
 * @brief Output header to configured output in configured format.
 * @param cfg
 */
void output_o_end(const config_t *cfg);

/**
 * @brief Configured output FILE*.
 */
extern FILE *out;

/**
 * @brief Configured error output FILE*.
 */
extern FILE *err;

/**
 * @brief Configured verbose output FILE*.
 */
extern FILE *verbose;

/**
 * @brief Initialize output based on cfg.
 * @param cfg
 * @return whether the initialization was successful
 */
bool output_init(const config_t *cfg);

/**
 * @brief Deinitialize output.
 */
void output_quit(void);

#endif  // ECGEN_OUTPUT_H
