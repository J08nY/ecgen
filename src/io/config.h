/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file config.h
 */
#ifndef ECGEN_CONFIG_H
#define ECGEN_CONFIG_H

#include <stdbool.h>
#include <stddef.h>

enum field_e { FIELD_PRIME, FIELD_BINARY };
enum format_e { FORMAT_JSON, FORMAT_CSV };
enum points_e {
	POINTS_PRIME,
	POINTS_RANDOM,
	POINTS_ALL,
	POINTS_NONPRIME,
	POINTS_NONE
};
struct points_s {
	enum points_e type;
	size_t amount;
};

typedef struct {
	enum field_e field;
	bool binary_field;
	bool prime_field;

	long count;
	bool random;
	bool prime;
	bool invalid;
	bool cm;
	char *order;
	bool anomalous;
	bool koblitz;
	bool cofactor;
	long cofactor_bound;
	bool from_seed;
	char *seed;
	bool unique;
	struct points_s points;

	char *datadir;
	unsigned long memory;
	unsigned long threads;
	unsigned long thread_memory;

	enum format_e format;
	char *output;
	char *input;
	bool append;

	long verbose;
	char *verbose_log;

	unsigned long bits;
	unsigned long hex_digits;

} config_t;

#endif  // ECGEN_CONFIG_H
