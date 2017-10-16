/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include <criterion/criterion.h>
#include "gen/field.h"
#include "math/poly.h"
#include "misc/types.h"
#include "test/io.h"

TestSuite(field, .init = io_setup, .fini = io_teardown);

Test(field, test_field_gen_random_fp) {
	curve_t curve = {0};
	cfg->bits = 10;
	cfg->prime_field = true;
	cfg->field = FIELD_PRIME;

	int ret = field_gen_random(&curve, NULL, OFFSET_FIELD);
	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.field, );
	cr_assert(gle(curve.field, int2n(cfg->bits)), );
}

Test(field, test_field_gen_random_f2m) {
	curve_t curve = {0};
	cfg->bits = 10;
	cfg->binary_field = true;
	cfg->field = FIELD_BINARY;

	int ret = field_gen_random(&curve, NULL, OFFSET_FIELD);
	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.field, );
	GEN coeffs = FF_to_FpXQ(curve.field);
	GEN vec = gtovec(coeffs);
	GEN n = fromdigits(vec, stoi(2));
	cr_assert(gle(n, int2n(cfg->bits)), );
}

Test(field, test_field_gen_input_fp) {
	curve_t curve = {0};
	cfg->bits = 10;
	cfg->prime_field = true;
	cfg->field = FIELD_PRIME;

	unsigned int p = 5;
	fprintf(write_in, "%u\n", p);
	int ret = field_gen_input(&curve, NULL, OFFSET_FIELD);
	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.field, );
	cr_assert(gequal(stoi(p), curve.field), );

	memset(&curve, 0, sizeof(curve_t));

	p = 4;
	fprintf(write_in, "%u\n", p);
	ret = field_gen_input(&curve, NULL, OFFSET_FIELD);
	cr_assert_eq(ret, 0, );
	cr_assert_null(curve.field, );

	memset(&curve, 0, sizeof(curve_t));

	p = 2 << 11;
	fprintf(write_in, "%u\n", p);
	ret = field_gen_input(&curve, NULL, OFFSET_FIELD);
	cr_assert_eq(ret, 0, );
	cr_assert_null(curve.field, );
};

Test(field, test_field_gen_input_f2m) {
	curve_t curve = {0};
	cfg->bits = 10;
	cfg->binary_field = true;
	cfg->field = FIELD_BINARY;

	unsigned int m = 9;
	fprintf(write_in, "%u\n", m);
	int ret = field_gen_input(&curve, NULL, OFFSET_FIELD);
	cr_assert_eq(ret, 0, );
	cr_assert_null(curve.field, );

	memset(&curve, 0, sizeof(curve_t));

	m = 10;
	unsigned int e1 = 17;
	fprintf(write_in, "%u\n%u\n", m, e1);
	ret = field_gen_input(&curve, NULL, OFFSET_FIELD);
	cr_assert_eq(ret, 0, );
	cr_assert_null(curve.field, );

	memset(&curve, 0, sizeof(curve_t));

	e1 = 9;
	unsigned int e2 = 17;
	fprintf(write_in, "%u\n%u\n%u\n", m, e1, e2);
	ret = field_gen_input(&curve, NULL, OFFSET_FIELD);
	cr_assert_eq(ret, 0, );
	cr_assert_null(curve.field, );

	memset(&curve, 0, sizeof(curve_t));

	e1 = 9;
	e2 = 5;
	unsigned int e3 = 17;
	fprintf(write_in, "%u\n%u\n%u\n%u\n", m, e1, e2, e3);
	ret = field_gen_input(&curve, NULL, OFFSET_FIELD);
	cr_assert_eq(ret, 0, );
	cr_assert_null(curve.field, );

	memset(&curve, 0, sizeof(curve_t));

	e1 = 0;
	e2 = 0;
	e3 = 0;
	fprintf(write_in, "%u\n%u\n%u\n%u\n", m, e1, e2, e3);
	ret = field_gen_input(&curve, NULL, OFFSET_FIELD);
	cr_assert_eq(ret, 0, );
	cr_assert_null(curve.field, );

	memset(&curve, 0, sizeof(curve_t));

	e1 = 4;
	fprintf(write_in, "%u\n%u\n%u\n%u\n", m, e1, e2, e3);
	ret = field_gen_input(&curve, NULL, OFFSET_FIELD);
	cr_assert_eq(ret, 0, );
	cr_assert_null(curve.field, );

	memset(&curve, 0, sizeof(curve_t));

	e1 = 3;
	fprintf(write_in, "%u\n%u\n%u\n%u\n", m, e1, e2, e3);
	ret = field_gen_input(&curve, NULL, OFFSET_FIELD);
	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.field, );
	GEN poly = poly_find_gen(m);
	cr_assert(gequal(poly, curve.field), );
}

Test(field, test_field_gen_once_fp) {
	curve_t curve = {0};
	cfg->bits = 10;
	cfg->prime_field = true;
	cfg->field = FIELD_PRIME;

	unsigned int p = 5;
	fprintf(write_in, "%u\n", p);
	int ret = field_gen_once(&curve, NULL, OFFSET_FIELD);
	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.field, );
	cr_assert(equalis(curve.field, p), );

	memset(&curve, 0, sizeof(curve_t));

	ret = field_gen_once(&curve, NULL, OFFSET_FIELD);
	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.field, );
	cr_assert(equalis(curve.field, p), );
	field_quit();
}

Test(field, test_field_gen_once_f2m) {
	curve_t curve = {0};
	cfg->bits = 10;
	cfg->binary_field = true;
	cfg->field = FIELD_BINARY;

	unsigned int m = 10;
	unsigned int e1 = 3;
	unsigned int e2 = 0;
	unsigned int e3 = 0;
	fprintf(write_in, "%u\n%u\n%u\n%u\n", m, e1, e2, e3);
	int ret = field_gen_once(&curve, NULL, OFFSET_FIELD);
	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.field, );
	GEN poly = poly_find_gen(m);
	cr_assert(gequal(poly, curve.field), );

	memset(&curve, 0, sizeof(curve_t));

	ret = field_gen_once(&curve, NULL, OFFSET_FIELD);
	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.field, );
	cr_assert(gequal(poly, curve.field), );
	field_quit();
}