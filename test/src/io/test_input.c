/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include "test/utils.h"
#include "io/input.h"
#include "io/output.h"
#include <criterion/criterion.h>

static FILE *write_in;

void input_setup() {
	default_setup();
	config_t cfg;
	memset(&cfg, 0, sizeof(cfg));

	cfg.output = "/dev/null";
	input_init(&cfg);
	output_init(&cfg);

	int in_fd[2];
	pipe(in_fd);

	write_in = fdopen(in_fd[1], "w");
	setlinebuf(write_in);
	in = fdopen(in_fd[0], "r");
}

void input_teardown() {
	default_teardown();
	input_quit();
	output_quit();
	fclose(write_in);
}

TestSuite(input, .init = input_setup, .fini = input_teardown);

Test(input, test_prime) {
	fprintf(write_in, "5\n");
	GEN p = input_prime(NULL, 10);
	cr_assert(gequal(p, stoi(5)),);
}

Test(input, test_prime_nan) {
	fprintf(write_in, "....\n");
	GEN p = input_prime(NULL, 10);
	cr_assert(gequal(p, gen_m1),);
}

Test(input, test_prime_nonprime) {
	fprintf(write_in, "6\n");
	GEN p = input_prime(NULL, 10);
	cr_assert(gequal(p, gen_m1),);
}

Test(input, test_int) {
	fprintf(write_in, "256\n");
	GEN i = input_int(NULL, 10);
	cr_assert(gequal(i, stoi(256)),);
}

Test(input, test_int_too_big) {
	fprintf(write_in, "256\n");
	GEN i = input_int(NULL, 4);
	cr_assert(gequal(i, gen_m1),);
}

Test(input, test_int_newline) {
	fprintf(write_in, "\n");
	GEN i = input_int(NULL, 4);
	cr_assert(gequal(i, gen_m1),);
}

Test(input, test_str) {
	fprintf(write_in, "something\n");
	GEN s = input_string(NULL);
	GEN expected = strtoGENstr("something");
	cr_assert(gequal(s, expected),);
}
