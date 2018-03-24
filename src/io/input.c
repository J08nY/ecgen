/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#define _POSIX_C_SOURCE 200809L

#include "input.h"
#include "output.h"

FILE *in;
int delim;

static GEN input_i(const char *prompt, unsigned long bits) {
	if (prompt && in == stdin) {
		fprintf(err, "%s ", prompt);
	}
	char *line = NULL;
	size_t n = 0;

	ssize_t len = getdelim(&line, &n, delim, in);
	if (len <= 0) {
		if (feof(in)) {
			fprintf(err, "Couldn't read an integer. Reached EOF!\n");
		} else if (ferror(in)) {
			perror("Couldn't read an integer.");
		} else {
			fprintf(err, "Couldn't read an integer.\n");
		}
		return gen_m2;
	}
	if (len == 1 && !feof(in)) {
		free(line);
		return gen_m1;
	}
	for (size_t i = 0, j = 0; (line[j] = line[i]); j += !isspace(line[i++]))
		;

	pari_sp ltop = avma;
	GEN in = strtoi(line);
	free(line);

	// check bitsize here
	GEN size = int2n(bits);
	if (cmpii(in, size) <= 0) {
		return gerepileupto(ltop, in);
	} else {
		fprintf(err, "Number too big(> %ld bits).\n", bits);
		return gen_m1;
	}
}

GEN input_prime(const char *prompt, unsigned long bits) {
	GEN read = input_i(prompt, bits);
	if (equalii(read, gen_m1) || equalii(read, gen_m2)) {
		return read;
	} else {
		if (isprime(read)) {
			return read;
		} else {
			fprintf(err, "Number is not prime. Prime required.\n");
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
		fprintf(err, "%s ", prompt);
	}
	char *line = NULL;
	size_t n = 0;

	ssize_t len = getdelim(&line, &n, delim, in);
	if (len <= 0) {
		return strtoGENstr("");
	}
	if (len == 1) {
		free(line);
		return strtoGENstr("");
	}

	line[len - 1] = 0;
	GEN result = strtoGENstr(line);
	free(line);
	return result;
}

bool input_init() {
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
	if (in != NULL && in != stdin) {
		fclose(in);
	}
}
