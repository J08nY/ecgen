/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <criterion/criterion.h>
#include "cm/custom.h"
#include "misc/config.h"
#include "test/default.h"
#include "test/input.h"
#include "util/random.h"

void custom_setup() {
	default_setup();
	input_setup();
	random_init();
}

void custom_teardown() { input_teardown(); }

TestSuite(custom, .init = custom_setup, .fini = custom_teardown);

Test(custom, test_curve_one) {
	cr_skip("Doesnt work yet.");
	cfg->bits = 128;
	cfg->cm_order = "263473633827487324648193013259296339349";
	cfg->random = false;

	fprintf(write_in, "191345683451069598953886857691544477637\n");
	custom_curve();
}

Test(custom, test_curve_other) {
	cr_skip("Doesnt work yet.");
	cfg->bits = 128;
	cfg->cm_order = "263473633827487324648193013259296339349";
	cfg->random = true;

	custom_curve();
}