/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file cli.h
 */
#ifndef ECGEN_CLI_H
#define ECGEN_CLI_H

#include <argp.h>
#include <stdbool.h>
#include <stdlib.h>

extern char doc[];
extern char args_doc[];
extern struct argp_option options[];

enum field_e { FIELD_PRIME, FIELD_BINARY };
enum format_e { FORMAT_JSON, FORMAT_CSV };
enum points_e { POINTS_PRIME, POINTS_RANDOM, POINTS_NONE };
struct points_s {
	enum points_e type;
	size_t amount;
};

typedef struct{
	enum field_e field;
	bool binary_field;
	bool prime_field;

	unsigned long count;
	bool random;
	bool prime;
	bool invalid;
	bool cm;
	char *order;
	bool koblitz;
	bool cofactor;
	long cofactor_bound;
	bool from_seed;
	char *seed;
	bool unique;
	struct points_s points;

	char *datadir;
	unsigned long memory;
	enum format_e format;
	char *output;
	char *input;
	bool append;

	long verbose;
	char *debug;

	long bits;

} config_t;

error_t cli_parse(int key, char *arg, struct argp_state *state);

char *cli_filter(int key, const char *text, void *input);

#endif  // ECGEN_CLI_H
