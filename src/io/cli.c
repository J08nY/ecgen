/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "cli.h"
#include <misc/config.h>
#include <string.h>
#include "exhaustive/ansi.h"
#include "exhaustive/brainpool.h"

char cli_doc[] =
    "ecgen, tool for generating Elliptic curve domain parameters.\v(C) 2017 "
    "Eastern Seaboard Phishing Authority";
char cli_args_doc[] = "bits";

enum opt_keys {
	OPT_DATADIR = 'd',
	OPT_COUNT = 'c',
	OPT_PRIME = 'p',
	OPT_COFACTOR = 'k',
	OPT_RANDOM = 'r',
	OPT_ANSI = 's',
	OPT_BRAINPOOL = 'b',
	OPT_INVALID = 'i',
	OPT_ORDER = 'n',
	OPT_KOBLITZ = 'K',
	OPT_UNIQUE = 'u',
	OPT_FORMAT = 't',
	OPT_OUTPUT = 'o',
	OPT_INPUT = 'f',
	OPT_APPEND = 'a',
	OPT_VERBOSE = 'v',
	OPT_MEMORY = 'm',
	OPT_FP = 1,
	OPT_F2M,
	OPT_POINTS,
	OPT_THREADS,
	OPT_TSTACK,
	OPT_TIMEOUT,
	OPT_ANOMALOUS,
	OPT_GPGEN,
	OPT_GPCHECK,
	OPT_HEXCHECK,
	OPT_BRAINPOOL_RFC,
	OPT_TWIST,
};

// clang-format off
struct argp_option cli_options[] = {
		{0,               0,                 0,        0,                 "Field specification:",                                                                  1},
		{"fp",            OPT_FP,            0,        0,                 "Prime field.",                                                                          1},
		{"f2m",           OPT_F2M,           0,        0,                 "Binary field.",                                                                         1},

		{0,               0,                 0,        0,                 "Generation methods:",                                                                   2},
		{"order",         OPT_ORDER,         "ORDER",  0,                 "Generate a curve with given order (using Complex Multiplication). **NOT IMPLEMENTED**", 2},
		{"anomalous",     OPT_ANOMALOUS,     0,        0,                 "Generate an anomalous curve (of trace one, with field order equal to curve order).",    2},
		{"ansi",          OPT_ANSI,          "SEED", OPTION_ARG_OPTIONAL, "Generate a curve from SEED (ANSI X9.62 verifiable procedure).",                         2},
		{"brainpool",     OPT_BRAINPOOL,     "SEED", OPTION_ARG_OPTIONAL, "Generate a curve from SEED (Brainpool procedure).",                                     2},
		{"brainpool-rfc", OPT_BRAINPOOL_RFC, "SEED", OPTION_ARG_OPTIONAL, "Generate a curve from SEED (Brainpool procedure, as per RFC 5639).",                    2},
		{"invalid",       OPT_INVALID,       0,        0,                 "Generate a set of invalid curves, for a given curve (using Invalid curve algorithm).",  2},
		{"twist",         OPT_TWIST,         0,        0,                 "Generate a twist of a given curve.",                                                    2},

		{0,               0,                 0,        0,                 "Generation options:",                                                                   3},
		{"random",        OPT_RANDOM,        0,        0,                 "Generate a random curve (using Random approach).",                                      3},
		{"prime",         OPT_PRIME,         0,        0,                 "Generate a curve with prime order.",                                                    3},
		{"cofactor",      OPT_COFACTOR,      "BOUND",  0,                 "Generate a curve with cofactor up to BOUND.",                                           3},
		{"koblitz",       OPT_KOBLITZ,       "A",    OPTION_ARG_OPTIONAL, "Generate a Koblitz curve (a in {0, 1}, b = 1).",                                        3},
		{"unique",        OPT_UNIQUE,        0,        0,                 "Generate a curve with only one generator.",                                             3},
		{"gp-gen",        OPT_GPGEN,         "FUNC",   0,                 "Generate a curve param using a GP function. **NOT IMPLEMENTED**",                       3},
		{"gp-check",      OPT_GPCHECK,       "FUNC",   0,                 "Check a generated curve param using a GP function. **NOT IMPLEMENTED**",                3},
		{"hex-check",     OPT_HEXCHECK,      "HEX",    0,                 "Check a generated curve param hex expansion for the HEX string.",                       3},
		{"points",        OPT_POINTS,        "TYPE",   0,                 "Generate points of given type (random/prime/all/nonprime/none).",                       3},
		{"count",         OPT_COUNT,         "COUNT",  0,                 "Generate multiple curves.",                                                             3},

		{0,               0,                 0,        0,                 "Input/Output options:",                                                                 4},
		{"format",        OPT_FORMAT,        "FORMAT", 0,                 "Format to output in. One of {csv, json}, default is json.",                             4},
		{"input",         OPT_INPUT,         "FILE",   0,                 "Input from file.",                                                                      4},
		{"output",        OPT_OUTPUT,        "FILE",   0,                 "Output into file. Overwrites any existing file!",                                       4},
		{"append",        OPT_APPEND,        0,        0,                 "Append to output file (don't overwrite).",                                              4},
		{"verbose",       OPT_VERBOSE,       "FILE", OPTION_ARG_OPTIONAL, "Verbose logging (to stdout or file).",                                                  4},

		{0,               0,                 0,        0,                 "Other:",                                                                                5},
		{"data-dir",      OPT_DATADIR,       "DIR",    0,                 "Set PARI/GP data directory (containing seadata package).",                              5},
		{"memory",        OPT_MEMORY,        "SIZE",   0,                 "Use PARI stack of SIZE (can have suffix k/m/g).",                                       5},
		{"threads",       OPT_THREADS,       "NUM",    0,                 "Use NUM threads.",                                                                      5},
		{"thread-stack",  OPT_TSTACK,        "SIZE",   0,                 "Use PARI stack of SIZE (per thread, can have suffix k/m/g).",                           5},
		{"timeout",       OPT_TIMEOUT,       "TIME",   0,                 "Timeout computation of a curve parameter after TIME (can have suffix s/m/h/d).",        5},
		{0}
};
// clang-format on

static unsigned long cli_parse_memory(const char *str,
                                      struct argp_state *state) {
	char *suffix = NULL;
	unsigned long read = strtoul(str, &suffix, 10);
	if (suffix == str) {
		argp_failure(state, 1, 0, "Wrong memory value.");
	}
	if (suffix) {
		if (*suffix == 'k' || *suffix == 'K') {
			read *= 1000;
		} else if (*suffix == 'm' || *suffix == 'M') {
			read *= 1000000;
		} else if (*suffix == 'g' || *suffix == 'G') {
			read *= 1000000000;
		}
	}
	return read;
}

static unsigned long cli_parse_time(const char *str, struct argp_state *state) {
	char *suffix = NULL;
	unsigned long read = strtoul(str, &suffix, 10);
	if (suffix == str) {
		argp_failure(state, 1, 0, "Wrong time value.");
	}
	if (suffix) {
		if (*suffix == 'm' || *suffix == 'M') {
			read *= 60;
		} else if (*suffix == 'h' || *suffix == 'H') {
			read *= 3600;
		} else if (*suffix == 'd' || *suffix == 'D') {
			read *= 86400;
		}
	}
	return read;
}

static void cli_end(struct argp_state *state) {
	// validate all option states here.
	// Only one field
	if (cfg->field == 0 || cfg->field == (FIELD_PRIME | FIELD_BINARY)) {
		argp_failure(state, 1, 0,
		             "Specify field type, prime or binary, with --fp / "
		             "--f2m (but not both).");
	}
	// Only one gen method
	switch (cfg->method) {
		case METHOD_DEFAULT:
		case METHOD_CM:
		case METHOD_ANOMALOUS:
		case METHOD_SEED:
		case METHOD_INVALID:
		case METHOD_TWIST:
			break;
		default:
			printf("%u\n", cfg->method);
			argp_failure(state, 1, 0,
			             "Only one generation method can be specified.");
			break;
	}

	if (cfg->method == METHOD_SEED && cfg->seed_algo == SEED_BRAINPOOL &&
	    cfg->field == FIELD_BINARY) {
		argp_failure(state, 1, 0,
		             "Brainpool algorithm only creates prime field curves.");
	}
	/*
	// Invalid is not prime or seed by definition.
	if (cfg->invalid &&
	    (cfg->prime || cfg->seed_algo || cfg->cofactor)) {
	    // not seed, not prime
	    argp_failure(state, 1, 0,
	                 "Invalid curve generation can not generate curves "
	                     "from seed, exhaustive or prime order.");
	}
	if (cfg->cm && (cfg->prime || cfg->seed_algo || cfg->invalid ||
	                cfg->cofactor || cfg->anomalous)) {
	    argp_failure(state, 1, 0,
	                 "Fixed order curve generation can not generate "
	                     "curves from seed, or invalid curves. Prime order "
	                     "also doesn't make sense if the given one isn't "
	                     "prime.");
	}
	if (cfg->anomalous &&
	    (cfg->binary_field || cfg->cofactor || cfg->seed_algo ||
	     cfg->cm || cfg->invalid || cfg->koblitz)) {
	    argp_failure(
	        state, 1, 0,
	        "Anomalous curve generation can not generate "
	            "binary field curves, curves with a cofactor, from seed "
	            "with fixed order, invalid or Koblitz curves.");
	}

	 */
	// default values
	if (!cfg->count) {
		cfg->count = 1;
	}
	if (!cfg->memory) {
		cfg->memory = 1000000000;
	}
	if (!cfg->threads) {
		cfg->threads = 1;
	}
	if (!cfg->thread_memory) {
		cfg->thread_memory = cfg->bits * 2000000;
	}
	if (!cfg->points.type) {
		cfg->points.type = POINTS_PRIME;
	}
}

error_t cli_parse(int key, char *arg, struct argp_state *state) {
	switch (key) {
		/* Field options */
		case OPT_FP:
			cfg->field |= FIELD_PRIME;
			break;
		case OPT_F2M:
			cfg->field |= FIELD_BINARY;
			break;

		/* Generation method  */
		case OPT_INVALID:
			cfg->method |= METHOD_INVALID;
			break;
		case OPT_ORDER:
			cfg->method |= METHOD_CM;
			if (arg) {
				cfg->cm_order = arg;
			}
			break;
		case OPT_ANOMALOUS:
			cfg->method |= METHOD_ANOMALOUS;
			break;
		case OPT_ANSI:
			cfg->method |= METHOD_SEED;
			cfg->seed_algo = SEED_ANSI;
			if (arg) {
				if (!ansi_seed_valid(arg)) {
					argp_failure(
					    state, 1, 0,
					    "SEED must be at least 160 bits (40 characters).");
				}
				cfg->seed = arg;
			}
			break;
		case OPT_BRAINPOOL:
			cfg->method |= METHOD_SEED;
			cfg->seed_algo = SEED_BRAINPOOL;
			if (arg) {
				if (!brainpool_seed_valid(arg)) {
					argp_failure(
					    state, 1, 0,
					    "SEED must be exactly 160 bits (40 hex characters).");
				}
				cfg->seed = arg;
			}
			break;
		case OPT_BRAINPOOL_RFC:
			cfg->method |= METHOD_SEED;
			cfg->seed_algo = SEED_BRAINPOOL_RFC;
			if (arg) {
				if (!brainpool_seed_valid(arg)) {
					argp_failure(
					    state, 1, 0,
					    "SEED must be exactly 160 bits (40 hex characters).");
				}
				cfg->seed = arg;
			}
			break;
		case OPT_TWIST:
			cfg->method |= METHOD_TWIST;
			break;

		/* Generation options */
		case OPT_COUNT:
			cfg->count = strtoul(arg, NULL, 10);
			break;
		case OPT_RANDOM:
			cfg->random = true;
			break;
		case OPT_PRIME:
			cfg->prime = true;
			break;
		case OPT_COFACTOR:
			cfg->cofactor = true;
			cfg->cofactor_bound = strtol(arg, NULL, 10);
			break;
		case OPT_KOBLITZ:
			cfg->koblitz = true;
			if (arg) {
				cfg->koblitz_value = strtol(arg, NULL, 10);
				if (cfg->koblitz_value != 0 && cfg->koblitz_value != 1) {
					argp_failure(state, 1, 0, "Wrong value for a = %li",
					             cfg->koblitz_value);
				}
			}
			break;
		case OPT_UNIQUE:
			cfg->unique = true;
			break;
		case OPT_GPGEN:
			cfg->gp_gens[cfg->gp_gens_size++] = arg;
			break;
		case OPT_GPCHECK:
			cfg->gp_checks[cfg->gp_checks_size++] = arg;
			break;
		case OPT_HEXCHECK: {
			char *str_start = arg;
			if (strlen(arg) > 2) {
				if (arg[0] == '0' && (arg[1] == 'x' || arg[1] == 'X')) {
					str_start = arg + 2;
				}
			}
			char *p = str_start;
			while (*p != 0) {
				char c = *p++;
				if (!isxdigit(c)) {
					argp_failure(
					    state, 1, 0,
					    "Hex check argument contains non hex char '%c'", c);
				}
			}
			cfg->hex_check = str_start;
			break;
		}
		case OPT_POINTS: {
			char *num_end;
			long amount = strtol(arg, &num_end, 10);
			cfg->points.amount = (size_t)amount;
			if (strstr(num_end, "random") == num_end) {
				cfg->points.type = POINTS_RANDOM;
			} else if (strstr(num_end, "prime") == num_end) {
				cfg->points.type = POINTS_PRIME;
			} else if (strstr(num_end, "all") == num_end) {
				cfg->points.type = POINTS_ALL;
			} else if (strstr(num_end, "nonprime") == num_end) {
				cfg->points.type = POINTS_NONPRIME;
			} else if (strstr(num_end, "none") == num_end) {
				cfg->points.type = POINTS_NONE;
			} else {
				argp_failure(state, 1, 0, "Unknown point type. %s", num_end);
			}
			break;
		}
		/* IO options */
		case OPT_FORMAT:
			if (!strcmp(arg, "csv")) {
				cfg->format = FORMAT_CSV;
			} else if (!strcmp(arg, "json")) {
				cfg->format = FORMAT_JSON;
			} else {
				argp_failure(state, 1, 0,
				             "Invalid format specified. One of [csv, json] "
				             "is valid.");
			}
			break;
		case OPT_INPUT:
			cfg->input = arg;
			break;
		case OPT_OUTPUT:
			cfg->output = arg;
			break;
		case OPT_APPEND:
			cfg->append = true;
			break;
		case OPT_VERBOSE:
			cfg->verbose++;
			if (arg) {
				cfg->verbose_log = arg;
			}
			break;

		/* Misc options */
		case OPT_DATADIR:
			cfg->datadir = arg;
			break;
		case OPT_MEMORY:
			cfg->memory = cli_parse_memory(arg, state);
			break;
		case OPT_TSTACK:
			cfg->thread_memory = cli_parse_memory(arg, state);
			break;
		case OPT_TIMEOUT:
			cfg->timeout = cli_parse_time(arg, state);
			break;
		case OPT_THREADS:
			if (!strcmp(arg, "auto") || !strcmp(arg, "AUTO")) {
				long nprocs = sysconf(_SC_NPROCESSORS_ONLN);
				if (nprocs > 0) {
					cfg->threads = (unsigned long)nprocs;
				}
			} else {
				cfg->threads = strtoul(arg, NULL, 10);
				if (!cfg->threads) {
					argp_failure(state, 1, 0,
					             "Invalid number of threads specified.");
				}
			}
			break;

		/* Args */
		case ARGP_KEY_ARG:
			if (state->arg_num >= 1) {
				argp_usage(state);
			}

			cfg->bits = strtoul(arg, NULL, 10);
			cfg->hex_digits =
			    2 * (cfg->bits / 8 + (cfg->bits % 8 != 0 ? 1 : 0));
			break;
		case ARGP_KEY_END:
			cli_end(state);
			break;
		case ARGP_KEY_NO_ARGS:
			argp_usage(state);
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

char *cli_filter(int key, const char *text, void *input) {
	return (char *)text;
}
