/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include "output.h"

char *output_scsv(const char *format, char delim, GEN vector) {
	long len = lg(vector) - 1;
	char *params[len];
	size_t lengths[len];
	size_t total = 0;
	for (long i = 0; i < len; ++i) {
		params[i] = pari_sprintf(format, gel(vector, i + 1));
		lengths[i] = strlen(params[i]);
		total += lengths[i];
	}

	char *result = (char *)malloc(total + len);
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
	memset(result + offset, 0, 1);

	return result;
}

void output_csv(FILE *out, const char *format, char delim, GEN vector) {
	char *string = output_scsv(format, delim, vector);
	fprintf(out, "%s", string);
	free(string);
}

char *output_sjson(GEN vector) {
}

void output_json(FILE *out, GEN vector) {}

FILE *output_open(const char *output, bool append) {
	if (output) {
		FILE *out = fopen(output, append ? "a" : "w");
		if (!out) {
			// fallback to stdout and output err
			out = stdout;
			perror("Failed to open output file.");
		}
		return out;
	} else {
		return stdout;
	}
}

void output_close(FILE *out) {
	if (out != NULL && out != stdout) {
		fclose(out);
	}
}