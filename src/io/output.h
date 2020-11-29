/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file output.h
 */
#ifndef ECGEN_IO_OUTPUT_H
#define ECGEN_IO_OUTPUT_H

#include <pari/pari.h>
#include <stdbool.h>
#include "misc/types.h"

#ifdef DEBUG

#include <inttypes.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <time.h>

#define _debug_print(prefix)                                               \
	fprintf(verbose, prefix);                                              \
	struct timespec ts;                                                    \
	clock_gettime(CLOCK_MONOTONIC, &ts);                                   \
	fprintf(verbose, "%d %" PRIdMAX ".%.9ld - ", (int)syscall(SYS_gettid), \
	        ts.tv_sec, ts.tv_nsec);

#define debug(...) pari_fprintf(verbose, __VA_ARGS__)
#define debug_log(...)          \
	do {                        \
		flockfile(verbose);     \
		_debug_print(" -  ");   \
		debug(__VA_ARGS__);     \
		fprintf(verbose, "\n"); \
		funlockfile(verbose);   \
	} while (0)

#define debug_log_start(...)    \
	do {                        \
		flockfile(verbose);     \
		_debug_print("[ ] ");   \
		debug(__VA_ARGS__);     \
		fprintf(verbose, "\n"); \
		funlockfile(verbose);   \
	} while (0)

#define debug_log_end(...)      \
	do {                        \
		flockfile(verbose);     \
		_debug_print("[*] ");   \
		debug(__VA_ARGS__);     \
		fprintf(verbose, "\n"); \
		funlockfile(verbose);   \
	} while (0)

#define debug_log_state(state, ...)                   \
	do {                                              \
		flockfile(verbose);                           \
		_debug_print(" -  ");                         \
		fprintf(verbose, "%s - ", offset_s[(state)]); \
		debug(__VA_ARGS__);                           \
		fprintf(verbose, "\n");                       \
		funlockfile(verbose);                         \
	} while (0)

#else
#define debug(...)
#define debug_log(...)
#define debug_log_start(...)
#define debug_log_end(...)
#define debug_log_state(...)
#endif  // DEBUG

#define verbose_log(...) \
	if (cfg->verbose) pari_fprintf(verbose, __VA_ARGS__)

#define output_log(...) pari_fprintf(out, __VA_ARGS__)

/**
 * @brief Output curve to a malloc'ed string in JSON format.
 * @param curve
 * @return
 */
char *output_sjson(curve_t *curve);

/**
 * @brief Output JSON separator(a ",\n") to a malloc'ed string in CSV
 * format.
 * @return
 */
char *output_sjson_separator();

/**
 * @brief Output JSON output header(a "[") to a malloc'ed string in CSV
 * format.
 * @return
 */
char *output_sjson_begin();

/**
 * @brief Output JSON output footer(a "]") to a malloc'ed string in CSV
 * format.
 * @return
 */
char *output_sjson_end();

/**
 * @brief Output curve to a malloc'ed string in configured format.
 * @param curve
 * @return
 */
extern char *(*output_s)(curve_t *curve);

/**
 * @brief Output curve to a FILE *out in configured format.
 * @param out
 * @param curve
 */
void output_f(FILE *out, curve_t *curve);

/**
 * @brief Output curve to configured output in configured format.
 * @param curve
 */
void output_o(curve_t *curve);

/**
 * @brief Output separator to a malloc'ed string in configured format.
 * @return
 */
extern char *(*output_s_separator)();

/**
 * @brief Output separator to a FILE *out in configured format.
 * @param out
 */
void output_f_separator(FILE *out);

/**
 * @brief Output separator to configured output in configured format.
 */
void output_o_separator();

/**
 * @brief Output header to a malloc'ed string in configured format.
 * @return
 */
extern char *(*output_s_begin)();

/**
 * @brief Output header to a FILE *out in configured format.
 * @param out
 */
void output_f_begin(FILE *out);

/**
 * @brief Output header to configured output in configured format.
 */
void output_o_begin();

/**
 * @brief Output footer to a malloc'ed string in configured format.
 * @return
 */
extern char *(*output_s_end)();

/**
 * @brief Output footer to a FILE *out in configured format.
 * @param out
 */
void output_f_end(FILE *out);

/**
 * @brief Output header to configured output in configured format.
 */
void output_o_end();

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
 * @return whether the initialization was successful
 */
bool output_init();

/**
 * @brief Deinitialize output.
 */
void output_quit(void);

#endif  // ECGEN_IO_OUTPUT_H
