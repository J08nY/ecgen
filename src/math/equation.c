/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "equation.h"
#include "io/input.h"
#include "math/field.h"

int a_random(curve_t *curve, const config_t *cfg, arg_t *args) {
	curve->a = genrand(curve->field);
	return 1;
}

int a_input(curve_t *curve, const config_t *cfg, arg_t *args) {
	pari_sp ltop = avma;
	GEN inp = input_int("a:", cfg->bits);
	if (gequalm1(inp)) {
		avma = ltop;
		return 0;
	}
	GEN elem = field_ielement(curve->field, inp);
	if (!elem) {
		avma = ltop;
		return 0;
	}
	curve->a = elem;
	return 1;
}

static GEN a = NULL;
static curve_t *curve_a = NULL;

int a_once(curve_t *curve, const config_t *cfg, arg_t *args) {
	if (a && curve_a == curve) {
		curve->a = gcopy(a);
		return 1;
	}

	int inp = a_input(curve, cfg, args);
	if (inp > 0) {
		a = gclone(curve->a);
		curve_a = curve;
		return 1;
	} else {
		return 0;
	}
}

int a_zero(curve_t *curve, const config_t *cfg, arg_t *args) {
	curve->a = gen_0;
	return 1;
}

int a_one(curve_t *curve, const config_t *cfg, arg_t *args) {
	curve->a = gen_1;
	return 1;
}

int a_seed(curve_t *curve, const config_t *cfg, arg_t *args) {
	// TODO implement
	return INT_MIN;
}

int b_random(curve_t *curve, const config_t *cfg, arg_t *args) {
	curve->b = genrand(curve->field);
	return 1;
}

int b_input(curve_t *curve, const config_t *cfg, arg_t *args) {
	pari_sp ltop = avma;
	GEN inp = input_int("b:", cfg->bits);
	if (gequalm1(inp)) {
		avma = ltop;
		return 0;
	}
	GEN elem = field_ielement(curve->field, inp);
	if (!elem) {
		avma = ltop;
		return 0;
	}
	curve->b = elem;
	return 1;
}

static GEN b = NULL;
static curve_t *curve_b = NULL;

int b_once(curve_t *curve, const config_t *cfg, arg_t *args) {
	if (b && curve_b == curve) {
		curve->b = gcopy(b);
		return 1;
	}

	int inp = b_input(curve, cfg, args);
	if (inp > 0) {
		b = gclone(curve->b);
		curve_b = curve;
		return 1;
	} else {
		return 0;
	}
}

int b_zero(curve_t *curve, const config_t *cfg, arg_t *args) {
	curve->b = gen_0;
	return 1;
}

int b_one(curve_t *curve, const config_t *cfg, arg_t *args) {
	curve->b = gen_1;
	return 1;
}

int b_seed(curve_t *curve, const config_t *cfg, arg_t *args) {
	// TODO implement
	return INT_MIN;
}

void equation_quit(void) {
	if (a && isclone(a)) {
		gunclone(a);
	}
	if (b && isclone(b)) {
		gunclone(b);
	}
}
