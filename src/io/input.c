/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#define _POSIX_C_SOURCE 200809L
#include "input.h"
#include "output.h"
#include "util/memory.h"

FILE *in;
int delim;

static GEN input_i(const char *prompt, unsigned long bits) {
	if (prompt && in == stdin) {
		fprintf(out, "%s ", prompt);
	}
	char *line = NULL;
	size_t n = 0;

	ssize_t len = getdelim(&line, &n, delim, in);
	if (len <= 0) {
		return gen_m1;
	}
	if (len == 1 && !feof(in)) {
		free(line);
		return gen_m1;
	}
	for (size_t i = 0, j = 0; (line[j] = line[i]); j += !isspace(line[i++]))
		;

	if (len <= 3 || !(line[0] == '0' && (line[1] == 'x' || line[1] == 'X'))) {
		char *new_line = try_realloc(line, (size_t)(len + 2));
		memmove(new_line + 2, new_line, (size_t)len);
		new_line[0] = '0';
		new_line[1] = 'x';
		if (!feof(in)) {
			new_line[len + 1] = 0;
		} else {
			new_line[len + 2] = 0;
		}
		line = new_line;
	}

	pari_sp ltop = avma;
	GEN in = strtoi(line);
	free(line);

	// check bitsize here
	GEN size = int2n(bits);
	if (cmpii(in, size) <= 0) {
		return gerepileupto(ltop, in);
	} else {
		fprintf(stderr, "Number too big(> %ld bits).\n", bits);
		return gen_m1;
	}
}

GEN input_prime(const char *prompt, unsigned long bits) {
	GEN read = input_i(prompt, bits);
	if (equalii(read, gen_m1)) {
		return read;
	} else {
		if (isprime(read)) {
			return read;
		} else {
			fprintf(stderr, "Number is not prime. Prime required.\n");
			return gen_m1;
		}
	}
}

GEN input_int(const char *prompt, unsigned long bits) {
	return input_i(prompt, bits);
}

GEN input_short(const char *prompt) { return input_i(prompt, 16); }

GEN input_string(const char *prompt) {
	if (prompt && in == stdin) {
		fprintf(out, "%s ", prompt);
	}
	char *line = NULL;
	size_t n = 0;

	ssize_t len = getdelim(&line, &n, delim, in);
	if (len == 1) {
		free(line);
		return strtoGENstr("");
	}

	line[len - 1] = 0;
	GEN result = strtoGENstr(line);
	free(line);
	return result;
}

bool input_init(const config_t *cfg) {
	if (cfg->input) {
		in = fopen(cfg->input, "r");
		delim = ',';
		if (!in) {
			perror("Failed to open input file.");
			return false;
		}
	} else {
		in = stdin;
		delim = '\n';
	}
	return true;
}

void input_quit(void) {
	if (in != NULL && in != stdout) {
		fclose(in);
	}
}
