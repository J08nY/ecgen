/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */

#include <criterion/criterion.h>
#include <criterion/parameterized.h>
#include "cm/supersingular.h"
#include "test/default.h"
#include "test/memory.h"

TestSuite(supersingular, .init = default_setup, .fini = default_teardown);

struct supersingular_params {
	char *p;
};

void supersingular_params_cleanup(struct criterion_test_params *ctp) {
	struct supersingular_params *params =
	    (struct supersingular_params *)ctp->params;
	cr_free(params->p);
}

ParameterizedTestParameters(supersingular, test_supersingular_gen_eq_examples) {
	static struct supersingular_params params[3] = {{0}};
	params[0].p = cr_strdup("0xd5a29f67");  // 0xd5a29f67 -> a = -1, b = 0
	params[1].p = cr_strdup("0x9420009d");  // 0x9420009d -> from j invariant
	params[2].p = cr_strdup("0xfac80035");  // 0xfac80035 -> a = 0, b = 1

	size_t nb_params = sizeof(params) / sizeof(struct supersingular_params);
	return cr_make_param_array(struct supersingular_params, params, nb_params,
	                           supersingular_params_cleanup);
}

ParameterizedTest(struct supersingular_params *param, supersingular,
                  test_supersingular_gen_eq_examples) {
	cfg->bits = 32;
	cfg->field = FIELD_PRIME;
	GEN p = strtoi(param->p);
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