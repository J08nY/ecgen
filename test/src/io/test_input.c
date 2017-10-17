/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <criterion/criterion.h>
#include "io/input.h"
#include "test/io.h"

TestSuite(input, .init = io_setup, .fini = io_teardown);

Test(input, test_prime) {
	fprintf(write_in, "5\n");
	GEN p = input_prime(NULL, 10);
	cr_assert(gequal(p, stoi(5)), );
}

Test(input, test_prime_nan) {
	fprintf(write_in, "....\n");
	GEN p = input_prime(NULL, 10);
	cr_assert(gequal(p, gen_m1), );
}

Test(input, test_prime_nonprime) {
	fprintf(write_in, "6\n");
	GEN p = input_prime(NULL, 10);
	cr_assert(gequal(p, gen_m1), );
}

Test(input, test_prime_newline) {
	fprintf(write_in, "\n");
	GEN p = input_prime(NULL, 10);
	cr_assert(gequal(p, gen_m1), );
}

Test(input, test_int) {
	fprintf(write_in, "256\n");
	GEN i = input_int(NULL, 10);
	cr_assert(gequal(i, stoi(256)), );
}

Test(input, test_int_too_big) {
	fprintf(write_in, "256\n");
	GEN i = input_int(NULL, 4);
	cr_assert(gequal(i, gen_m1), );
}

Test(input, test_int_newline) {
	fprintf(write_in, "\n");
	GEN i = input_int(NULL, 4);
	cr_assert(gequal(i, gen_m1), );
}

Test(input, test_str) {
	fprintf(write_in, "something\n");
	GEN s = input_string(NULL);
	GEN expected = strtoGENstr("something");
	cr_assert(gequal(s, expected), );
}

Test(input, test_str_newline) {
	fprintf(write_in, "\n");
	GEN s = input_string(NULL);
	GEN expected = strtoGENstr("");
	cr_assert(gequal(s, expected), );
}
