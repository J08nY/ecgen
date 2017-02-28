/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "cli.h"
#include <string.h>

char doc[] =
	"ecgen, tool for generating Elliptic curve domain parameters.\v(C) 2017 "
		"Eastern Seaboard Phishing Authority";
char args_doc[] = "bits";

enum opt_keys {
	OPT_DATADIR = 'd',
	OPT_COUNT = 'c',
	OPT_PRIME = 'p',
	OPT_RANDOM = 'r',
	OPT_SEED = 's',
	OPT_INVALID = 'i',
	OPT_ORDER = 'n',
	OPT_KOBLITZ = 'k',
	OPT_UNIQUE = 'u',
	OPT_FORMAT = 't',
	OPT_OUTPUT = 'o',
	OPT_INPUT = 'f',
	OPT_APPEND = 'a',
	OPT_VERBOSE = 'v',
	OPT_FP = 1,
	OPT_F2M = 2,
};

// clang-format off
struct argp_option options[] = {
	{0,          0,           0,        0,                 "Field specification:",                                              1},
	{"fp",       OPT_FP,      0,        0,                 "Prime field.",                                                      1},
	{"f2m",      OPT_F2M,     0,        0,                 "Binary field.",                                                     1},
	{0,          0,           0,        0,                 "Generation options:",                                               2},
	{"random",   OPT_RANDOM,  0,        0,                 "Generate a random curve.",                                          2},
	{"prime",    OPT_PRIME,   0,        0,                 "Generate a curve with prime order.",                                2},
	{"seed",     OPT_SEED,    "SEED", OPTION_ARG_OPTIONAL, "Generate a curve from SEED (ANSI X9.62 verifiable procedure).",     2},
	{"invalid",  OPT_INVALID, 0,        0,                 "Generate a set of invalid curves (for a given curve).",             2},
	{"order",    OPT_ORDER,   "ORDER",  0,                 "Generate a curve with given order (using Complex Multiplication).", 2},
	{"koblitz",  OPT_KOBLITZ, 0,        0,                 "Generate a Koblitz curve (a = 0).",                                 2},
	{"unique",   OPT_UNIQUE,  0,        0,                 "Generate a curve with only one generator.",                         2},
	//{"count",    OPT_COUNT,   "COUNT",  0,              "Generate multiple (COUNT) curves."},
	{0,          0,           0,        0,                 "Input/Output options:",                                             3},
	{"format",   OPT_FORMAT,  "FORMAT", 0,                 "Format to output in. One of [csv,json], default is json.",          3},
	{"input",    OPT_INPUT,   "FILE",   0,                 "Input from file.",                                                  3},
	{"output",   OPT_OUTPUT,  "FILE",   0,                 "Output into file. Overwrites any existing file!",                   3},
	{"append",   OPT_APPEND,  0,        0,                 "Append to output file (don't overwrite).",                          3},
	{"verbose",  OPT_VERBOSE, "FILE", OPTION_ARG_OPTIONAL, "Verbose logging (to stdout or file).",                              3},
	{0,          0,           0,        0,                 "Other:",                                                            4},
	{"data-dir", OPT_DATADIR, "DIR",    0,                 "PARI/GP data directory (containing seadata package).",              4},
	{0}
};
// clang-format on

error_t cli_parse(int key, char *arg, struct argp_state *state) {
	struct config_t *cfg = state->input;

	switch (key) {
		case OPT_DATADIR: cfg->datadir = arg;
			break;
		case OPT_COUNT:
			if (arg) {
				cfg->count = strtol(arg, NULL, 10);
			}
			break;
		case OPT_FORMAT:
			if (arg) {
				if (!strcmp(arg, "csv")) {
					cfg->format = FORMAT_CSV;
				} else if (!strcmp(arg, "json")) {
					cfg->format = FORMAT_JSON;
				} else {
					argp_failure(state, 1, 0,
								 "Invalid format specified. One of [csv, json] "
									 "is valid.");
				}
			} else {
				argp_failure(
					state, 1, 0,
					"You have to specify a format with the format option.");
			}
			break;
		case OPT_INPUT: cfg->input = arg;
			break;
		case OPT_OUTPUT: cfg->output = arg;
			break;
		case OPT_APPEND: cfg->append = true;
			break;
		case OPT_VERBOSE: cfg->verbose++;
			if (arg) {
				cfg->debug = arg;
			}
			break;
		case OPT_RANDOM: cfg->random = true;
			break;
		case OPT_PRIME: cfg->prime = true;
			break;
		case OPT_INVALID: cfg->invalid = true;
			break;
		case OPT_ORDER: cfg->cm = true;
			if (arg) {
				cfg->order = arg;
			}
			break;
		case OPT_KOBLITZ: cfg->koblitz = true;
			break;
		case OPT_UNIQUE: cfg->unique = true;
			break;
		case OPT_SEED: cfg->from_seed = true;
			if (arg) {
				// ANSI X9.62 specifies seed as at least 160 bits in length.
				if (strlen(arg) < 20) {
					argp_failure(
						state, 1, 0,
						"SEED must be at least 160 bits (20 characters).");
				}
				cfg->seed = arg;
			}
			break;
		case OPT_FP: cfg->field = FIELD_PRIME;
			cfg->prime_field = true;
			break;
		case OPT_F2M: cfg->field = FIELD_BINARY;
			cfg->binary_field = true;
			break;
		case ARGP_KEY_ARG:
			if (state->arg_num >= 1) {
				argp_usage(state);
			}

			cfg->bits = strtol(arg, NULL, 10);
			break;
		case ARGP_KEY_END:
			// validate all option states here.
			// Only one field
			if (!cfg->prime_field && !cfg->binary_field) {
				argp_failure(state, 1, 0,
							 "Specify field type, prime or binary, with --fp / "
								 "--f2m (but not both).");
			}
			// Invalid is not prime or seed by definition.
			if (cfg->invalid && (cfg->prime || cfg->from_seed)) {
				// not seed, not prime
				argp_failure(state, 1, 0,
							 "Invalid curve generation can not generate curves "
								 "from seed, exhaustive or prime order.");
			}
			if (cfg->cm && (cfg->prime || cfg->from_seed || cfg->invalid)) {
				argp_failure(state, 1, 0,
							 "Fixed order curve generation can not generate "
								 "curves from seed, or invalid curves. Prime order "
								 "also doesn't make sense if the given one isn't "
								 "prime.");
			}
			// default values
			if (!cfg->count) {
				cfg->count = 1;
			}
			break;
		case ARGP_KEY_NO_ARGS: argp_usage(state);
			break;
		default: return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

char *cli_filter(int key, const char *text, void *input) {
	return (char *)text;
}
