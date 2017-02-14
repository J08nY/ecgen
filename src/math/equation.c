/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "equation.h"
#include "io/input.h"

int eq_random(curve_t *curve, config_t *config, ...) {
	int r = a_random(curve, config) + b_random(curve, config);
	if (r == 2) {
		return r;
	}
	return -1;
}

int a_random(curve_t *curve, config_t *config, ...) {
	curve->a = genrand(curve->field);
	return 1;
}

int a_input(curve_t *curve, config_t *config, ...) {
	curve->a = fread_int(in, "a:", config->bits);
	return 1;
}

int a_zero(curve_t *curve, config_t *config, ...) {
	curve->a = gen_0;
	return 1;
}

int a_one(curve_t *curve, config_t *config, ...) {
	curve->a = gen_1;
	return 1;
}

int a_seed(curve_t *curve, config_t *config, ...) {}

int b_random(curve_t *curve, config_t *config, ...) {
	curve->b = genrand(curve->field);
	return 1;
}

int b_input(curve_t *curve, config_t *config, ...) {
	curve->b = fread_int(in, "b:", config->bits);
	return 1;
}

int b_zero(curve_t *curve, config_t *config, ...) {
	curve->b = gen_0;
	return 1;
}

int b_one(curve_t *curve, config_t *config, ...) {
	curve->b = gen_1;
	return 1;
}

int b_seed(curve_t *curve, config_t *config, ...) {}
