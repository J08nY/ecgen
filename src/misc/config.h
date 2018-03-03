/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file config.h
 */
#ifndef ECGEN_CONFIG_H
#define ECGEN_CONFIG_H

#include <stdbool.h>
#include <stddef.h>

enum field_e { FIELD_PRIME = 1 << 0, FIELD_BINARY = 1 << 1 };
enum format_e { FORMAT_JSON };
enum points_e {
	POINTS_NONE = 0,
	POINTS_PRIME,
	POINTS_RANDOM,
	POINTS_ALL,
	POINTS_NONPRIME
};
struct points_s {
	enum points_e type;
	size_t amount;
};

/**
 * @brief
 */
typedef enum {
	METHOD_DEFAULT = 0,
	METHOD_CM = 1 << 0,
	METHOD_ANOMALOUS = 1 << 1,
	METHOD_SEED = 1 << 2,
	METHOD_INVALID = 1 << 3,
	METHOD_TWIST
} method_e;

/**
 * @brief
 */
typedef enum {
	SEED_NONE = 0,
	SEED_ANSI,
	SEED_BRAINPOOL,
	SEED_BRAINPOOL_RFC,
	SEED_FIPS
} seed_e;

/**
 * @brief
 */
typedef struct {
	/** @brief What field should the curves be generated over. */
	enum field_e field;

	/** @brief Generation method. */
	method_e method;

	/** @brief How many curves should be generated. */
	long count;
	/** @brief Whether the curves should be generated at random (no input). */
	bool random;
	/** @brief Whether the curves should have prime order. */
	bool prime;
	/** @brief Whether the Complex Multiplication method should be used. */
	char *cm_order;
	/** @brief Whether the curves should be Koblitz (a \\in {0, 1}, b = 1). */
	bool koblitz;
	long koblitz_value;
	/** @brief Whether the curves should have a bound on the cofactor value. */
	bool cofactor;
	long cofactor_value;
	/** @brief What seed algorithm, if any, to use to generate the curves. */
	seed_e seed_algo;
	/** @brief What seed to use, if any, to generate the curves. */
	char *seed;
	/** @brief Whether the curves should be uniquely generated (one generator).
	 */
	bool unique;
	/** @brief The GP gen functions. */
	char *gp_gens[10];
	size_t gp_gens_size;
	/** @brief The GP check functions. */
	char *gp_checks[10];
	size_t gp_checks_size;
	/** @brief */
	char *hex_check;
	/** @brief What points to generate on the curves. */
	struct points_s points;

	/** @brief The datadir to use, if any. */
	char *datadir;
	/** @brief How much memory to allocate for the PARI stack. */
	unsigned long memory;
	/** @brief How many threads to use, only useful for invalid generation(atm).
	 */
	unsigned long threads;
	/** @brief How much memory to allocate for the PARI stack, per thread. */
	unsigned long thread_memory;
	/** @brief How long of a timeout interval, if any, to give to parameter
	 * generation. */
	unsigned long timeout;

	/** @brief What output format to use. */
	enum format_e format;
	/** @brief What, if any, output file to write to. */
	char *output;
	/** @brief What, if any, input file to read from. */
	char *input;
	/** @brief Whether to append or truncate the output file on output. */
	bool append;

	/** @brief What verbosity level to run on. */
	long verbose;
	/** @brief What file, if any, to write the verbose messages. */
	char *verbose_log;

	/** @brief The size of the field to generate curves over, in bits. */
	unsigned long bits;
	unsigned long hex_digits;
} config_t;

extern config_t cfg_s;
extern config_t *cfg;

#endif  // ECGEN_CONFIG_H
