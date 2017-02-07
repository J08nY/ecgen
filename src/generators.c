/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "generators.h"
#include "curve.h"
#include "equation.h"
#include "field.h"
#include "seed.h"

int gen_skip(curve_t *curve, config_t *config) { return 1; }

void gen_init(gen_t generators[], config_t *config) {
	if (config->from_seed) {
		if (config->seed) {
			generators[OFFSET_SEED] = &seed_argument;
		} else {
			if (config->random) {
				generators[OFFSET_SEED] = &seed_random;
			} else {
				generators[OFFSET_SEED] = &seed_input;
			}
		}
		generators[OFFSET_A] = &a_seed;
		generators[OFFSET_B] = &b_seed;
		generators[OFFSET_CURVE] = &curve_seed;
	} else {
		generators[OFFSET_SEED] = &gen_skip;
		generators[OFFSET_A] = &a_random;
		generators[OFFSET_B] = &b_random;
		if (config->prime) {
			generators[OFFSET_CURVE] = &curve_prime;
		} else {
			generators[OFFSET_CURVE] = &curve_nonzero;
		}
	}

	generators[OFFSET_FIELD] = &field_random;
}