/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "input.h"

FILE *in;

GEN fread_i(FILE *stream, const char *prompt, long bits, int delim,
            GEN (*rand_func)(long)) {
	if (prompt) {
		printf("%s ", prompt);
	}
	char *line = NULL;
	size_t n = 0;

	ssize_t len = getdelim(&line, &n, delim, stream);
	if (len == 1) {
		free(line);
		if (rand_func) {
			return rand_func(bits);
		} else {
			return gen_0;
		}
	}
	pari_sp ltop = avma;
	GEN in = strtoi(line);
	free(line);

	// check bitsize here
	GEN size = int2n(bits);
	if (cmpii(in, size)) {
		return gerepileupto(ltop, in);
	} else {
		fprintf(stderr, "Number too big(> %ld bits).\n", bits);
		return gen_m1;
	}
}

GEN fread_prime(FILE *stream, const char *prompt, long bits, int delim) {
	GEN read = fread_i(stream, prompt, bits, delim, &random_prime);
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

GEN fread_int(FILE *stream, const char *prompt, long bits, int delim) {
	return fread_i(stream, prompt, bits, delim, &random_int);
}

GEN fread_short(FILE *stream, const char *prompt, int delim) {
	return fread_i(stream, prompt, 16, delim, NULL);
}

GEN fread_string(FILE *stream, const char *prompt, int delim) {
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

GEN fread_param(param_t param, FILE *stream, const char *prompt, long bits,
                int delim) {
	switch (param) {
		case PARAM_PRIME:
			return fread_prime(stream, prompt, bits, delim);
		case PARAM_INT:
			return fread_int(stream, prompt, bits, delim);
		case PARAM_SHORT:
			return fread_short(stream, prompt, delim);
		case PARAM_STRING:
			return fread_string(stream, prompt, delim);
	}
	return gen_m1;
}

GEN read_param(param_t param, const char *prompt, long bits, int delim) {
	return fread_param(param, stdin, prompt, bits, delim);
}

FILE *input_open(const char *input) {
	if (input) {
		FILE *in = fopen(input, "r");
		if (!in) {
			// fallback to stdin or quit?
			in = stdin;
			perror("Failed to open input file.");
		}
		return in;
	} else {
		return stdin;
	}
}

void input_close(FILE *in) {
	if (in != NULL && in != stdout) {
		fclose(in);
	}
}
