/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */

#include <criterion/criterion.h>
#include "exhaustive/brainpool.h"
#include "gen/seed.h"
#include "test/io.h"
#include "util/bits.h"
#include "util/memory.h"

TestSuite(brainpool, .init = io_setup, .fini = io_teardown);

Test(brainpool, test_brainpool_seed_random) {
	curve_t curve = {0};
	cfg->bits = 256;
	int ret = brainpool_gen_seed_random(&curve, NULL, OFFSET_SEED);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );

	seed_free(&curve.seed);
}

Test(brainpool, test_brainpool_seed_argument) {
	curve_t curve = {0};
	char *seed = "abcdefabcdefabcdefabcdefabcdefabcdefabcd";
	cfg->seed = seed;
	cfg->bits = 256;
	int ret = brainpool_gen_seed_argument(&curve, NULL, OFFSET_SEED);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );
	char *hex = bits_to_hex(curve.seed->seed);
	cr_assert_str_eq(hex, seed, );

	try_free(hex);
	seed_free(&curve.seed);
}

Test(brainpool, test_brainpool_seed_input) {
	curve_t curve = {0};
	char *seed = "abcdefabcdefabcdefabcdefabcdefabcdefabcd";
	cfg->bits = 256;
	fprintf(write_in, "%s\n", seed);
	int ret = brainpool_gen_seed_input(&curve, NULL, OFFSET_SEED);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );
	char *hex = bits_to_hex(curve.seed->seed);
	cr_assert_str_eq(hex, seed, );

	try_free(hex);
	seed_free(&curve.seed);
}

Test(brainpool, test_brainpool_seed_input_short) {
	curve_t curve = {0};
	char *seed = "abcdef";
	fprintf(write_in, "%s\n", seed);
	int ret = brainpool_gen_seed_input(&curve, NULL, OFFSET_SEED);

	cr_assert_eq(ret, 0, );
}

Test(brainpool, test_brainpool_field) {
	curve_t curve = {0};
	char *seed = "abcdefabcdefabcdefabcdefabcdefabcdefabcd";
	cfg->seed = seed;
	cfg->bits = 256;

	brainpool_gen_seed_argument(&curve, NULL, OFFSET_SEED);

	int ret = brainpool_gen_field(&curve, NULL, OFFSET_FIELD);
	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.field, );
	cr_assert(isprime(gcopy(curve.field)), );
	cr_assert_eq(mod4(curve.field), 3, );
}

Test(brainpool, test_brainpool_equation) {
	curve_t curve = {0};
	char *seed = "abcdefabcdefabcdefabcdefabcdefabcdefabcd";
	cfg->seed = seed;
	cfg->bits = 256;

	brainpool_gen_seed_argument(&curve, NULL, OFFSET_SEED);

	char *prime =
	    "caa002c44829499e8239f049bddcaca373258a175fa337a82e0521744392cff1";
	bits_t *prime_bits = bits_from_hex(prime);
	curve.field = bits_to_i(prime_bits);
	bits_free(&prime_bits);

	int ret = brainpool_gen_equation(&curve, NULL, OFFSET_B);
	cr_assert_eq(ret, 1, );
}