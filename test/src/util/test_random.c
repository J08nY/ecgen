/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <criterion/criterion.h>
#include "test/default.h"
#include "util/random.h"

void random_setup() {
	default_setup();
	random_init();
}

TestSuite(random, .init = random_setup, .fini = default_teardown);

Test(random, test_random_prime) {
	for (size_t i = 0; i < 100; ++i) {
		GEN p = random_prime(10);
		cr_assert(isprime(p), );
		cr_assert_leq(cmpii(p, int2n(10)), 0, );
		cr_assert_geq(cmpii(p, int2n(9)), 0, );
	}
}

Test(random, test_random_int) {
	for (size_t i = 0; i < 100; ++i) {
		GEN j = random_int(10);
		cr_assert_leq(cmpii(j, int2n(10)), 0, );
		cr_assert_geq(cmpii(j, int2n(9)), 0, );
	}
}
