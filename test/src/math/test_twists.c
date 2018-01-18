/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include <criterion/criterion.h>
#include "gen/point.h"
#include "gen/field.h"
#include "math/poly.h"
#include "math/twists.h"
#include "test/default.h"
#include "util/random.h"

TestSuite(twists, .init = default_setup, .fini = default_teardown);

Test(twists, test_twist_rand_fp) {
	random_init();
	GEN a = mkintmodu(3, 23);
	GEN b = mkintmodu(4, 23);
	curve_t curve = {.a = a,
	                 .b = b,
	                 .field = stoi(23),
	                 .curve = ellinit(mkvec2(a, b), stoi(23), -1)};

	curve_t to = {0};
	curve_twist_rand(&to, &curve);
	cr_assert_not_null(to.a, );
	cr_assert_not_null(to.b, );
	cr_assert_not_null(to.field, );
	cr_assert_not_null(to.curve, );
}

Test(twists, test_twist_rand_f2m) {
	random_init();
	GEN field = poly_find_gen(13);
	GEN a = field_ielement(field, stoi(2));
	GEN b = field_ielement(field, stoi(3));
	curve_t curve = {.a = a,
					 .b = b,
					 .field = field,
					 .curve = ellinit(mkvecn(5, gen_1, a, gen_0, gen_0, b), NULL, -1)};

	curve_t to = {0};
	curve_twist_rand(&to, &curve);
	cr_assert_not_null(to.a, );
	cr_assert_not_null(to.b, );
	cr_assert_not_null(to.field, );
	cr_assert_not_null(to.curve, );
}