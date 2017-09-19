/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "output.h"
#include "misc/types.h"
#include "io/output.h"

FILE *read_out = NULL;
FILE *read_err = NULL;
FILE *read_verbose = NULL;

static void setup_stream(FILE **original_out, FILE **redirected_out) {
	int fd[2];
	pipe(fd);

	*redirected_out = fdopen(fd[0], "r");
	setlinebuf(*redirected_out);
	*original_out = fdopen(fd[1], "w");
	setlinebuf(*original_out);
}

void output_setup(void) {
	config_t cfg;
	memset(&cfg, 0, sizeof(cfg));
	output_init(&cfg);

	int in_fd[2];
	pipe(in_fd);

	setup_stream(&out, &read_out);
	setup_stream(&err, &read_err);
	setup_stream(&verbose, &read_verbose);
}

void output_teardown(void) {
	if (read_out) {
		fclose(out);
		fclose(read_out);
	}
	if (read_err) {
		fclose(err);
		fclose(read_err);
	}
	if (read_verbose) {
		fclose(verbose);
		fclose(read_verbose);
	}
}
