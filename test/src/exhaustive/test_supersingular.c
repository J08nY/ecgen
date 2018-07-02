/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */

#include <criterion/criterion.h>
#include "exhaustive/supersingular.h"
#include "test/default.h"

TestSuite(supersingular, .init = default_setup, .fini = default_teardown);

Test(supersingular, test_supersingular_gen_eq) {
	GEN p = strtoi("0xebc040451686221f");
	curve_t curve = {.field = p};

	int ret = supersingular_gen_equation(&curve, NULL, OFFSET_B);
	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.a, );
	cr_assert_not_null(curve.b, );

	GEN e = ellinit(mkvec2(curve.a, curve.b), p, -1);
	cr_assert_neq(glength(e), 0, );
	GEN c = ellcard(e, NULL);
	cr_assert(equalii(addis(p, 1), c), );
}
