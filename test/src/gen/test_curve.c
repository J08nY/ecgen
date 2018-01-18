/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include <criterion/criterion.h>
#include "gen/curve.h"
#include "test/default.h"

TestSuite(curve, .init = default_setup, .fini = default_teardown);

Test(curve, test_curve_new) {
	curve_t *curve = curve_new();
	cr_assert_not_null(curve, );
	curve_free(&curve);
	cr_assert_null(curve, );
}

Test(curve, test_curve_copy) {
	curve_t *curve = curve_new();
	curve_t *other = curve_new_copy(curve);
	cr_assert_not_null(other, );
	curve_copy(curve, other);
	cr_assert_not_null(other, );
	curve_free(&curve);
	curve_free(&other);
}

Test(curve, test_curve_clone) {
	curve_t *curve = curve_new();
	curve_t *other = curve_new_clone(curve);
	cr_assert_not_null(other, );
	curve_clone(curve, other);
	cr_assert_not_null(other, );
	curve_free(&curve);
	curve_free(&other);
}