/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "equation.h"
#include "field.h"
#include "io/input.h"

GENERATOR(a_gen_random) {
	curve->a = genrand(curve->field);
	return 1;
}

GENERATOR(a_gen_input) {
	pari_sp ltop = avma;
	GEN inp = input_int("a:", cfg->bits);
	if (gequalm1(inp)) {
		avma = ltop;
		return 0;
	} else if (equalii(inp, gen_m2)) {
		avma = ltop;
		return INT_MIN;
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

GENERATOR(a_gen_once) {
	if (a && curve_a == curve) {
		curve->a = gcopy(a);
		return 1;
	}

	int inp = a_gen_input(curve, args, state);
	if (inp > 0) {
		a = gclone(curve->a);
		curve_a = curve;
		return 1;
	} else {
		return inp;
	}
}

GENERATOR(a_gen_zero) {
	curve->a = gen_0;
	return 1;
}

GENERATOR(a_gen_one) {
	curve->a = gen_1;
	return 1;
}

GENERATOR(b_gen_random) {
	curve->b = genrand(curve->field);
	return 1;
}

GENERATOR(b_gen_input) {
	pari_sp ltop = avma;
	GEN inp = input_int("b:", cfg->bits);
	if (gequalm1(inp)) {
		avma = ltop;
		return 0;
	} else if (equalii(inp, gen_m2)) {
		avma = ltop;
		return INT_MIN;
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

GENERATOR(b_gen_once) {
	if (b && curve_b == curve) {
		curve->b = gcopy(b);
		return 1;
	}

	int inp = b_gen_input(curve, args, state);
	if (inp > 0) {
		b = gclone(curve->b);
		curve_b = curve;
		return 1;
	} else {
		return inp;
	}
}

GENERATOR(b_gen_zero) {
	curve->b = gen_0;
	return 1;
}

GENERATOR(b_gen_one) {
	curve->b = gen_1;
	return 1;
}

void equation_quit(void) {
	if (a && isclone(a)) {
		gunclone(a);
	}
	if (b && isclone(b)) {
		gunclone(b);
	}
}
