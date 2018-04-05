/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include <criterion/criterion.h>
#include "gen/point.h"
#include "math/koblitz.h"
#include "test/default.h"

TestSuite(koblitz, .init = default_setup, .fini = default_teardown);

Test(koblitz, test_koblitz_is_curve) {
	curve_t curve = {
	    .field = stoi(23), .a = mkintmodu(3, 23), .b = mkintmodu(2, 23)};

	cr_assert_not(koblitz_is_curve(&curve), );
	curve.a = mkintmodu(1, 23);
	curve.b = mkintmodu(1, 23);
	cr_assert(koblitz_is_curve(&curve), );
}

Test(koblitz, test_koblitz_find) {
	const koblitz_t *koblitz = koblitz_find(107, 0);
	cr_assert_not_null(koblitz, );
}

Test(koblitz, test_koblitz_get_order) {
	GEN order = koblitz_get_order(107, 0);
	cr_assert(gequal(order, strtoi("0x7ffffffffffffb57c25324737c4")), );
}