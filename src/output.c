/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "output.h"

void output_csv(FILE *out, char delim, char format, GEN vector) {
	char fmt[] = {'%', 'P', format, 0};

	long len = lg(vector) - 1;
	char *params[len];
	size_t lengths[len];
	size_t total = 0;
	for (long i = 0; i < len; ++i) {
		params[i] = pari_sprintf(fmt, gel(vector, i + 1));
		lengths[i] = strlen(params[i]);
		total += lengths[i];
	}

	char *result = malloc(total + len);
	if (!result) {
		perror("Couldn't malloc.");
		exit(1);
	}

	size_t offset = 0;
	for (long i = 0; i < len; ++i) {
		memcpy(result + offset, params[i], lengths[i]);
		free(params[i]);

		offset += lengths[i];
		if (i != len - 1) {
			result[offset] = delim;
			offset++;
		}
	}
	result[offset] = 0;

	fprintf(out, "%s", result);
	free(result);
}

