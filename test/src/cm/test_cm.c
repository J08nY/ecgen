/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */

#include <criterion/criterion.h>
#include "cm/cm_any.h"
#include "cm/cm_prime.h"
#include "obj/curve.h"
#include "test/default.h"
#include "test/input.h"
#include "test/output.h"
#include "util/random.h"

void cm_setup() {
	default_setup();
	input_setup();
	output_setup();
	random_init();
}

void cm_teardown() {
	default_teardown();
	input_teardown();
	output_teardown();
}

TestSuite(cm, .init = cm_setup, .fini = cm_teardown);

Test(cm, test_curve_prime) {
	cfg->bits = 128;
	cfg->cm_order = "263473633827487324648193013259296339349";
	GEN order = strtoi(cfg->cm_order);

	curve_t *curve = cm_prime_curve(order);
	cr_assert_not_null(curve, );
	cr_assert(equalii(curve->order, order), );
	cr_assert(equalii(ellcard(curve->curve, NULL), order), );
	curve_free(&curve);
}

Test(cm, test_curve_composite) {
	cfg->bits = 64;
	cfg->cm_order = "13282407956253574712";
	GEN order = strtoi(cfg->cm_order);

	curve_t *curve = cm_any_curve(order);
	cr_assert_not_null(curve, );
	cr_assert(equalii(curve->order, order), );
	cr_assert(equalii(ellcard(curve->curve, NULL), order), );
	curve_free(&curve);
}