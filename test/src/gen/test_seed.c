/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <criterion/criterion.h>
#include "gen/seed.h"
#include "gen/types.h"
#include "test/default.h"
#include "test/input.h"
#include "test/output.h"

void seed_suite_setup(void) {
	default_setup();
	input_setup();
	output_setup();
}

void seed_suite_teardown(void) {
	default_teardown();
	input_teardown();
	output_teardown();
}

TestSuite(seed, .init = seed_suite_setup, .fini = seed_suite_teardown);

Test(seed, test_seed_random) {
	curve_t curve = {};
	config_t cfg = {};
	int ret = seed_gen_random(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );
	cr_assert_str_eq(curve.seed->raw, pari_sprintf("%Px", curve.seed->seed), );
	cr_assert_eq(strlen(curve.seed->raw), curve.seed->raw_len, );
}

Test(seed, test_seed_argument) {
	curve_t curve = {};
	char *seed = "abcdefabcdefabcdefab";
	config_t cfg = {.seed = seed};
	int ret = seed_gen_argument(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );
	cr_assert_str_eq(curve.seed->raw, seed, );
	cr_assert_str_eq(curve.seed->raw, pari_sprintf("%Px", curve.seed->seed), );
	cr_assert_eq(strlen(curve.seed->raw), curve.seed->raw_len, );
}

Test(seed, test_seed_argument_hex) {
	curve_t curve = {};
	char *seed = "0xabcdefabcdefabcdefab";
	config_t cfg = {.seed = seed};
	int ret = seed_gen_argument(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );
	cr_assert_str_eq(curve.seed->raw, pari_sprintf("%Px", curve.seed->seed), );
	cr_assert_eq(strlen(curve.seed->raw), curve.seed->raw_len, );
}

Test(seed, test_seed_input) {
	curve_t curve = {};
	char *seed = "abcdefabcdefabcdefab";
	config_t cfg = {};
	fprintf(write_in, "%s\n", seed);
	int ret = seed_gen_input(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );
	cr_assert_str_eq(curve.seed->raw, seed, );
	cr_assert_str_eq(curve.seed->raw, pari_sprintf("%Px", curve.seed->seed), );
	cr_assert_eq(strlen(curve.seed->raw), curve.seed->raw_len, );
}

Test(seed, test_seed_input_short) {
	curve_t curve = {};
	char *seed = "abcdef";
	config_t cfg = {};
	fprintf(write_in, "%s\n", seed);
	int ret = seed_gen_input(&curve, &cfg, NULL);

	cr_assert_eq(ret, 0, );
}