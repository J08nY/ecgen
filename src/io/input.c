/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "input.h"
#include <parson/parson.h>

FILE *in;
int delim;

GEN fread_i(FILE *stream, const char *prompt, long bits) {
	if (prompt) {
		printf("%s ", prompt);
	}
	char *line = NULL;
	size_t n = 0;

	ssize_t len = getdelim(&line, &n, delim, stream);
	if (len == 1) {
		free(line);
		return gen_m1;
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

GEN fread_prime(FILE *stream, const char *prompt, long bits) {
	GEN read = fread_i(stream, prompt, bits);
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

GEN fread_int(FILE *stream, const char *prompt, long bits) {
	return fread_i(stream, prompt, bits);
}

GEN fread_short(FILE *stream, const char *prompt) {
	return fread_i(stream, prompt, 16);
}

GEN fread_string(FILE *stream, const char *prompt) {
	if (prompt) {
		printf("%s ", prompt);
	}
	char *line = NULL;
	size_t n = 0;

	ssize_t len = getdelim(&line, &n, delim, stream);
	if (len == 1) {
		free(line);
		return strtoGENstr("");
	}

	line[len - 1] = 0;
	GEN result = strtoGENstr(line);
	free(line);
	return result;
}

GEN fread_param(param_t param, FILE *stream, const char *prompt, long bits) {
	switch (param) {
		case PARAM_PRIME:
			return fread_prime(stream, prompt, bits);
		case PARAM_INT:
			return fread_int(stream, prompt, bits);
		case PARAM_SHORT:
			return fread_short(stream, prompt);
		case PARAM_STRING:
			return fread_string(stream, prompt);
	}
	return gen_m1;
}

GEN read_param(param_t param, const char *prompt, long bits) {
	return fread_param(param, stdin, prompt, bits);
}

void input_init(const char *input) {
	json_set_allocation_functions(pari_malloc, pari_free);

	if (input) {
		in = fopen(input, "r");
		delim = ',';
		if (!in) {
			// fallback to stdin or quit?
			in = stdin;
			delim = '\n';
			perror("Failed to open input file.");
		}
	} else {
		in = stdin;
		delim = '\n';
	}
}

void input_quit() {
	if (in != NULL && in != stdout) {
		fclose(in);
	}
}
