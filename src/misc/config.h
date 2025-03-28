/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file config.h
 */
#ifndef ECGEN_MISC_CONFIG_H
#define ECGEN_MISC_CONFIG_H

#include <stdbool.h>
#include <stddef.h>

enum field_e { FIELD_PRIME = 1 << 0, FIELD_BINARY = 1 << 1 };
enum format_e { FORMAT_JSON };
enum points_e {
	POINTS_PRIME = 0,
	POINTS_NONPRIME,
	POINTS_RANDOM,
	POINTS_ALL,
	POINTS_NONE
};
struct points_s {
	enum points_e type;
	size_t amount;
};

/**
 * @brief
 */
typedef enum {
	RANDOM_NONE = 0,
	RANDOM_SEED = 1 << 0,
	RANDOM_FIELD = 1 << 1,
	RANDOM_A = 1 << 2,
	RANDOM_B = 1 << 3,
	RANDOM_EQUATION = RANDOM_A | RANDOM_B,
	RANDOM_ALL = RANDOM_SEED | RANDOM_FIELD | RANDOM_EQUATION
} random_e;

/**
 * @brief
 */
typedef enum {
	METHOD_DEFAULT = 0,
	METHOD_CM = 1 << 0,
	METHOD_ANOMALOUS = 1 << 1,
	METHOD_SEED = 1 << 2,
	METHOD_INVALID = 1 << 3,
	METHOD_TWIST = 1 << 4,
	METHOD_SUPERSINGULAR = 1 << 5,
	METHOD_FAMILY = 1 << 6
} method_e;

/**
 * @brief
 */
typedef enum {
	SEED_NONE = 0,
	SEED_ANSI,
	SEED_BRAINPOOL,
	SEED_BRAINPOOL_RFC,
	SEED_FIPS,
	SEED_NUMS,
	SEED_FAMILY
} seed_e;

/**
 * @brief
 */
typedef enum {
	FAMILY_BN = 0,
	FAMILY_BLS12,
	FAMILY_BLS24,
	FAMILY_KSS16,
	FAMILY_KSS18,
	FAMILY_KSS36,
	FAMILY_KSS40
} family_e;

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
	/** @brief What parameters should be generated at random (no input). */
	random_e random;
	/** @brief Whether the curves should have prime order. */
	bool prime;
	/** @brief Whether the Complex Multiplication method should be used. */
	char *cm_order;
	/** @brief Whether the curves should be Koblitz (a \\in {0, 1}, b = 1). */
	bool koblitz;
	long koblitz_value;
	/** @brief Whether the curves should have a smooth order (bit-length bound).
	 */
	bool smooth;
	long smooth_value;
	/** @brief Whether the curves should have a bound on the cofactor value. */
	bool cofactor;
	long cofactor_value;
	/** @brief A range of prime orders that should be generated in invalid
	 * generation. */
	char *invalid_primes;
	/** @brief What seed algorithm, if any, to use to generate the curves. */
	seed_e seed_algo;
	/** @brief What seed to use, if any, to generate the curves. */
	char *seed;
	/** @brief What family of curves to generate. */
	family_e family;
	/** @brief Whether the curves should be uniquely generated (one generator).
	 */
	bool unique;
	/** @brief */
	char *hex_check;
	/** @brief What points to generate on the curves. */
	struct points_s points;
	/** @brief Compute curve metadata. */
	bool metadata;

	/** @brief The datadir to use, if any. */
	char *datadir;
	/** @brief How much memory to allocate for the PARI stack. */
	unsigned long memory;
	/** @brief How many threads to use (or limit PARI to use).
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

typedef struct {
	bool field;
	bool method;
	bool count;
	bool random;
	bool prime;
	bool cm_order;
	bool koblitz;
	bool koblitz_value;
	bool smooth;
	bool cofactor;
	bool invalid_primes;
	bool seed_algo;
	bool seed;
	bool family;
	bool unique;
	bool hex_check;
	bool points;
	bool metadata;
} config_names_t;

extern config_names_t cfg_used_s;
extern config_names_t *cfg_used;

extern config_names_t cfg_set_s;
extern config_names_t *cfg_set;

#define GET(x) cfg_used->x = true
#define GET_BOOL(x) ((cfg_used->x = true) && cfg->x)
#define SET(x) cfg_set->x = true

void config_report_unused(void);

#endif  // ECGEN_MISC_CONFIG_H
