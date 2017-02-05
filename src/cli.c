/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "cli.h"

char doc[] = "ecgen, tool for generating Elliptic curve domain parameters.\v(C) 2017 Eastern Seaboard Phishing Authority";
char args_doc[] = "bits";

enum opt_keys {
	OPT_DATADIR = 'd',
	OPT_APPEND = 'a',
	OPT_PRIME = 'p',
	OPT_OUTPUT = 'o',
	OPT_INPUT = 'i',
	OPT_RANDOM = 'r',
	OPT_FP = 1,
	OPT_F2M = 2,
};

struct argp_option options[] = {
	// Field specification
	{"fp",       OPT_FP,      0,      0, "Prime field."},
	{"f2m",      OPT_F2M,     0,      0, "Binary field."},
	// Curve specification
	{"random",   OPT_RANDOM,  0,      0, "Generate a random curve."},
	{"prime",    OPT_PRIME,   0,      0, "Generate a curve with prime order."},
	// Other
	{"data-dir", OPT_DATADIR, "DIR",  0,
										 "PARI/GP data directory (containing seadata and elldata)."},
	{"input",    OPT_INPUT,   "FILE", 0, "Input from file."},
	{"output",   OPT_OUTPUT,  "FILE", 0, "Output into file. Overwrites any existing file!"},
	{"append",   OPT_APPEND,  0,      0, "Append to output file(don't overwrite)."},
	{0}};

error_t parse_opt(int key, char *arg, struct argp_state *state) {
	struct arguments *args = state->input;

	switch (key) {
		case OPT_DATADIR:
			args->datadir = arg;
			break;
		case OPT_INPUT:
			args->input = arg;
			break;
		case OPT_OUTPUT:
			args->output = arg;
			break;
		case OPT_APPEND:
			args->append = true;
			break;
		case OPT_RANDOM:
			args->random = true;
			break;
		case OPT_PRIME:
			args->prime = true;
			break;
		case OPT_FP:
			if (args->binary_field) {
				argp_failure(
					state, 1, 0,
					"Either specify prime field or binary field, not both.");
			}
			args->field = FIELD_PRIME;
			args->prime_field = true;
			break;
		case OPT_F2M:
			if (args->prime_field) {
				argp_failure(
					state, 1, 0,
					"Either specify binary field or prime field, not both.");
			}
			args->field = FIELD_BINARY;
			args->binary_field = true;
			break;
		case ARGP_KEY_ARG:
			if (state->arg_num >= 1) {
				argp_usage(state);
			}

			args->bits = strtol(arg, NULL, 10);
			break;
		case ARGP_KEY_END:
			if (!args->prime_field && !args->binary_field) {
				argp_failure(
					state, 1, 0,
					"Specify field type, prime or binary, with --fp / --f2m.");
			}
			break;
		case ARGP_KEY_NO_ARGS:
			argp_usage(state);
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}
