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
	GEN vec = gtocol0(gen_0, 2);
	gel(vec, 1) = stoi(2);
	gel(vec, 2) = stoi(3);
	cr_assert(gequal(divs, vec), "Factors not equal!");
}

Test(subgroups, test_prime_prime) {
	curve_t curve = {.order = stoi(5)};
	config_t cfg = {.prime = true};
	GEN divs = subgroups_prime(&curve, &cfg);
	GEN vec = gtocol(stoi(5));
	cr_assert(gequal(divs, vec), "Factors not equal!");
}