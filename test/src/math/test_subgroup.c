/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include <criterion/criterion.h>
#include "gen/point.h"
#include "math/subgroup.h"
#include "test/default.h"

TestSuite(subgroup, .init = default_setup, .fini = default_teardown);

Test(subgroup, test_prime_factors) {
	GEN divs = subgroups_prime(stoi(12));
	GEN vec = mkvec2s(2, 3);
	cr_assert(gequal(divs, vec), "Factors not equal!");
}

Test(subgroup, test_prime_factors_other) {
	GEN divs = subgroups_prime(stoi(27));
	GEN vec = gtovec(stoi(3));
	cr_assert(gequal(divs, vec), "Factors not equal!");
}

Test(subgroup, test_prime_prime) {
	GEN divs = subgroups_prime(stoi(5));
	GEN vec = gtovec(stoi(5));
	cr_assert(gequal(divs, vec), "Factors not equal!");
}

Test(subgroup, test_nonprime_factors) {
	GEN divs = subgroups_nonprime(stoi(27));
	GEN vec = mkvec2s(9, 27);
	cr_assert(gequal(divs, vec), "Factors not equal!");
}

Test(subgroup, test_all_factors) {
	GEN divs = subgroups_all(stoi(27));
	GEN vec = mkvec3s(3, 9, 27);
	cr_assert(gequal(divs, vec), "Factors not equal!");
}