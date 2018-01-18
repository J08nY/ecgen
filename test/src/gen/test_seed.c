/*
 * ecgen, tool for generating Elliptic seed domain parameters
 * Copyright (C) 2017 J08nY
 */
#include <criterion/criterion.h>
#include "gen/seed.h"
#include "test/default.h"

TestSuite(seed, .init = default_setup, .fini = default_teardown);

Test(seed, test_seed_new) {
	seed_t *seed = seed_new();
	cr_assert_not_null(seed, );
	seed_free(&seed);
	cr_assert_null(seed, );
}

Test(seed, test_seed_copy) {
	seed_t *seed = seed_new();
	seed_t *other = seed_new_copy(seed);
	cr_assert_not_null(other, );
	seed_copy(seed, other);
	cr_assert_not_null(other, );
	seed_free(&seed);
	seed_free(&other);
}

Test(seed, test_seed_clone) {
	seed_t *seed = seed_new();
	seed_t *other = seed_new_clone(seed);
	cr_assert_not_null(other, );
	seed_clone(seed, other);
	cr_assert_not_null(other, );
	seed_free(&seed);
	seed_free(&other);
}