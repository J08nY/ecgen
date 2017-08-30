/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include "test/utils.h"
#include "util/random.h"
#include <criterion/criterion.h>

void random_setup() {
	default_setup();
	random_init();
}

TestSuite(random, .init = random_setup, .fini = default_teardown);

Test(random, test_random_prime) {
	for (size_t i = 0; i < 100; ++i) {
		GEN p = random_prime(10);
		cr_assert(isprime(p),);
		cr_assert_lt(gcmp(p, int2n(10)), 0,);
		cr_assert_gt(gcmp(p, int2n(9)), 0,);
	}
}

Test(random, test_random_int) {
	for (size_t i = 0; i < 100; ++i) {
		GEN j = random_int(10);
		cr_assert_lt(gcmp(j, int2n(10)), 0,);
		cr_assert_gt(gcmp(j, int2n(9)), 0,);
	}
}


