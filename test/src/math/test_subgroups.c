/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "gen/types.h"
#include "math/subgroups.h"
#include <criterion/criterion.h>
#include "test/utils.h"

TestSuite(subgroups, .init=default_setup, .fini=default_teardown);

Test(subgroups, test_prime_factors) {
	curve_t curve = {.order = stoi(12)};
	config_t cfg = {.prime = false};
	GEN divs = subgroups_prime(&curve, &cfg);
	GEN vec = mkvec2s(2, 3);
	cr_assert(gequal(divs, vec), "Factors not equal!");
}

Test(subgroups, test_prime_factors_other) {
	curve_t curve = {.order = stoi(27)};
	config_t cfg = {.prime = false};
	GEN divs = subgroups_prime(&curve, &cfg);
	GEN vec = gtovec(stoi(3));
	cr_assert(gequal(divs, vec), "Factors not equal!");
}

Test(subgroups, test_prime_prime) {
	curve_t curve = {.order = stoi(5)};
	config_t cfg = {.prime = true};
	GEN divs = subgroups_prime(&curve, &cfg);
	GEN vec = gtovec(stoi(5));
	cr_assert(gequal(divs, vec), "Factors not equal!");
}

Test(subgroups, test_nonprime_factors) {
	// curve = ellinit([1, 3], 23), order = 27
	curve_t curve = {.order = stoi(27), .ngens = 1};
	config_t cfg = {.prime = false};
	GEN divs = subgroups_nonprime(&curve, &cfg);
	GEN vec = mkvec2s(9, 27);
	cr_assert(gequal(divs, vec),
			  "Factors not equal!");
}

Test(subgroups, test_all_factors) {
	// curve = ellinit([1, 3], 23), order = 27
	curve_t curve = {.order = stoi(27), .ngens = 1};
	config_t cfg = {.prime = false};
	GEN divs = subgroups_all(&curve, &cfg);
	GEN vec = mkvec3s(3, 9, 27);
	cr_assert(gequal(divs, vec), "Factors not equal!");
}
