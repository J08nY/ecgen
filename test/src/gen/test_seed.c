/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <criterion/criterion.h>
#include "gen/seed.h"
#include "gen/types.h"
#include "test/default.h"

TestSuite(seed, .init = default_setup, .fini = default_teardown);

Test(seed, test_seed_random) {
	curve_t curve = {};
	config_t cfg = {};
	int ret = seed_gen_random(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );
	cr_assert_str_eq(pari_sprintf("%Px", curve.seed->seed), curve.seed->raw, );
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
	cr_assert_str_eq(pari_sprintf("%Px", curve.seed->seed), curve.seed->raw, );
	cr_assert_eq(strlen(curve.seed->raw), curve.seed->raw_len, );
}

Test(seed, test_seed_input) {}