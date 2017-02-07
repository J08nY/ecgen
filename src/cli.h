/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
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

typedef struct config_t {
	enum field_e field;
	bool binary_field;
	bool prime_field;
	bool random;
	bool prime;
	bool invalid;
	bool from_seed;
	char *seed;
	char *datadir;
	char *output;
	char *input;
	bool append;
	long bits;
} config_t;

error_t parse_opt(int key, char *arg, struct argp_state *state);

#endif  // ECGEN_CLI_H
