/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <criterion/criterion.h>
#include "cm/custom.h"
#include "obj/curve.h"
#include "test/default.h"
#include "test/input.h"
#include "test/output.h"
#include "util/random.h"

void custom_setup() {
	default_setup();
	input_setup();
	output_setup();
	random_init();
}

void custom_teardown() {
	default_teardown();
	input_teardown();
	output_teardown();
}

TestSuite(custom, .init = custom_setup, .fini = custom_teardown);

Test(custom, test_curve_one) {
	cfg->bits = 128;
	cfg->cm_order = "263473633827487324648193013259296339349";
	GEN order = strtoi(cfg->cm_order);
	cfg->random = RANDOM_ALL;

	curve_t *curve = custom_curve();
	cr_assert_not_null(curve, );
	cr_assert(equalii(curve->order, order), );
	cr_assert(equalii(ellcard(curve->curve, NULL), order), );
	curve_free(&curve);
}