/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include <criterion/criterion.h>
#include "gen/equation.h"
#include "test/default.h"
#include "test/input.h"

void equation_setup() {
	default_setup();
	input_setup();
}

void equation_teardown() {
	default_teardown();
	input_teardown();
}

TestSuite(equation, .init = equation_setup, .fini = equation_teardown);

Test(equation, test_a_gen_random) {
	curve_t curve = {.field = stoi(19)};

	int ret = a_gen_random(&curve, NULL, OFFSET_A);
	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.a, );
}

Test(equation, test_a_gen_input) {
	curve_t curve = {.field = stoi(19)};
	cfg->bits = 10;

	unsigned int i = 7;
	fprintf(write_in, "%u\n", i);
	int ret = a_gen_input(&curve, NULL, OFFSET_A);
	cr_assert_eq(ret, 1, );
	cr_assert(gequal(modsi(i, curve.field), curve.a), );
}

Test(equation, test_a_gen_once) {
	curve_t curve = {.field = stoi(19)};
	cfg->bits = 10;

	unsigned int i = 7;
	GEN modi = modsi(i, curve.field);
	fprintf(write_in, "%u\n", i);
	int ret = a_gen_once(&curve, NULL, OFFSET_A);
	cr_assert_eq(ret, 1, );
	cr_assert(gequal(modi, curve.a), );

	ret = a_gen_once(&curve, NULL, OFFSET_A);
	cr_assert_eq(ret, 1, );
	cr_assert(gequal(modi, curve.a), );
	equation_quit();
}

Test(equation, test_a_gen_zero) {
	curve_t curve = {0};
	int ret = a_gen_zero(&curve, NULL, OFFSET_A);
	cr_assert_eq(ret, 1, );
	cr_assert(gequal(gen_0, curve.a), );
}

Test(equation, test_a_gen_one) {
	curve_t curve = {0};
	int ret = a_gen_one(&curve, NULL, OFFSET_A);
	cr_assert_eq(ret, 1, );
	cr_assert(gequal(gen_1, curve.a), );
}

Test(equation, test_b_gen_random) {
	curve_t curve = {.field = stoi(19)};

	int ret = b_gen_random(&curve, NULL, OFFSET_B);
	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.b, );
}

Test(equation, test_b_gen_input) {
	curve_t curve = {.field = stoi(19)};
	cfg->bits = 10;

	unsigned int i = 7;
	fprintf(write_in, "%u\n", i);
	int ret = b_gen_input(&curve, NULL, OFFSET_B);
	cr_assert_eq(ret, 1, );
	cr_assert(gequal(modsi(i, curve.field), curve.b), );
}

Test(equation, test_b_gen_once) {
	curve_t curve = {.field = stoi(19)};
	cfg->bits = 10;

	unsigned int i = 7;
	GEN modi = modsi(i, curve.field);
	fprintf(write_in, "%u\n", i);
	int ret = b_gen_once(&curve, NULL, OFFSET_B);
	cr_assert_eq(ret, 1, );
	cr_assert(gequal(modi, curve.b), );

	ret = b_gen_once(&curve, NULL, OFFSET_B);
	cr_assert_eq(ret, 1, );
	cr_assert(gequal(modi, curve.b), );
	equation_quit();
}

Test(equation, test_b_gen_zero) {
	curve_t curve = {0};
	int ret = b_gen_zero(&curve, NULL, OFFSET_B);
	cr_assert_eq(ret, 1, );
	cr_assert(gequal(gen_0, curve.b), );
}

Test(equation, test_b_gen_one) {
	curve_t curve = {0};
	int ret = b_gen_one(&curve, NULL, OFFSET_B);
	cr_assert_eq(ret, 1, );
	cr_assert(gequal(gen_1, curve.b), );
}
