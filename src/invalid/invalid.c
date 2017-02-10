/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "invalid.h"
#include "math/curve.h"
#include "math/field.h"
#include "math/equation.h"

int invalid_do(config_t *cfg) {
	// create the curve to invalidate
	// Either from input or random with -r

	curve_t *curve = curve_new();
	gen_t gen[OFFSET_END];
	gen[OFFSET_SEED] = &gen_skip;
	if (cfg->random) {
		gen[OFFSET_FIELD] = &field_random;
		gen[OFFSET_A] = &a_random;
		gen[OFFSET_B] = &b_random;
	} else {
		gen[OFFSET_FIELD] = &field_input;
		gen[OFFSET_A] = &a_input;
		gen[OFFSET_B] = &b_input;
	}
	curve_free(&curve);

	return 0;
}