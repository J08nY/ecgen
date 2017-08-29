/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include <criterion/criterion.h>
#include "gen/point.h"
#include "math/subgroups.h"
#include "test/utils.h"

TestSuite(subgroups, .init = default_setup, .fini = default_teardown);

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
	cr_assert(gequal(divs, vec), "Factors not equal!");
}

Test(subgroups, test_all_factors) {
	// curve = ellinit([1, 3], 23), order = 27
	curve_t curve = {.order = stoi(27), .ngens = 1};
	config_t cfg = {.prime = false};
	GEN divs = subgroups_all(&curve, &cfg);
	GEN vec = mkvec3s(3, 9, 27);
	cr_assert(gequal(divs, vec), "Factors not equal!");
}

Test(subgroups, test_all_factors_two_gens) {
	// curve = ellinit([2, 3], 23), order = 24
	point_t **gens = points_new(2);
	gens[0] = point_new();
	gens[0]->order = stoi(12);
	gens[0]->cofactor = stoi(2);
	gens[0]->point = mkvec2(mkintmodu(6, 23), mkintmodu(1, 23));
	gens[1] = point_new();
	gens[1]->order = stoi(6);
	gens[1]->cofactor = stoi(4);
	gens[1]->point = mkvec2(mkintmodu(20, 23), mkintmodu(19, 23));

	curve_t curve = {.order = stoi(24), .ngens = 2, .generators = gens};
	config_t cfg = {.prime = false};
	GEN divs = subgroups_all(&curve, &cfg);
	GEN vec = mkvecn(8, stoi(2), stoi(3), stoi(4), stoi(6), stoi(12), stoi(2),
	                 stoi(3), stoi(6));
	cr_assert(gequal(divs, vec), "Factors not equal!");
	points_free_deep(&gens, 2);
}
