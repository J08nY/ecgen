/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include <criterion/criterion.h>
#include "gen/gp.h"
#include "test/default.h"
#include "util/bits.h"

TestSuite(gp, .init = default_setup, .fini = default_teardown);

Test(gp, test_gp_seed) {
	curve_t curve = {0};
	config_t cfg = {};
	arg_t arg = {.args = "() -> { return(Vecsmall([1,0])); }", .nargs = 1};

	int ret = gp_gen(&curve, &cfg, &arg, OFFSET_SEED);
	cr_assert_eq(ret, 1,);
	cr_assert_not_null(curve.seed,);
	cr_assert_not_null(curve.seed->seed,);
	cr_assert(bits_eq(curve.seed->seed, bits_from_bin("10")),);
}

Test(gp, test_gp_field) {
	curve_t curve = {0};
	config_t cfg = {};
	arg_t arg = {.args = "(seed) -> { return(19); }", .nargs = 1};

	int ret = gp_gen(&curve, &cfg, &arg, OFFSET_FIELD);
	cr_assert_eq(ret, 1,);
	cr_assert(gequal(curve.field, stoi(19)),);
}

Test(gp, test_gp_a) {
	curve_t curve = {.field = stoi(19)};
	config_t cfg = {};
	arg_t arg = {.args = "(seed, field) -> { return(Mod(3,field)); }", .nargs = 1};

	int ret = gp_gen(&curve, &cfg, &arg, OFFSET_A);
	cr_assert_eq(ret, 1,);
	cr_assert(gequal(curve.a, mkintmodu(3, 19)),);
}

Test(gp, test_gp_b) {
	curve_t curve = {.field = stoi(19), .a = mkintmodu(3, 19)};
	config_t cfg = {};
	arg_t arg = {.args = "(seed, field, a) -> { return(a * 2); }", .nargs = 1};

	int ret = gp_gen(&curve, &cfg, &arg, OFFSET_B);
	cr_assert_eq(ret, 1,);
	cr_assert(gequal(curve.b, mkintmodu(6, 19)),);
}

Test(gp, test_gp_curve) {
	curve_t curve = {.field = stoi(19), .a = mkintmodu(3, 19), .b = mkintmodu(6, 19)};
	config_t cfg = {};
	arg_t arg = {.args = "(seed, field, a, b) -> { return(ellinit([a,b], field)); }", .nargs = 1};

	int ret = gp_gen(&curve, &cfg, &arg, OFFSET_CURVE);
	cr_assert_eq(ret, 1,);
	cr_assert(gequal(curve.curve, ellinit(mkvec2(curve.a, curve.b), curve.field, 0)),);
}

Test(gp, test_gp_order) {
	curve_t curve = {.field = stoi(19), .a = mkintmodu(3, 19), .b = mkintmodu(6, 19),
	.curve = ellinit(mkvec2(stoi(3), stoi(6)), stoi(19), 0)};
	config_t cfg = {};
	arg_t arg = {.args = "(seed, field, a, b, curve) -> { return(ellsea(curve)); }", .nargs = 1};

	int ret = gp_gen(&curve, &cfg, &arg, OFFSET_ORDER);
	cr_assert_eq(ret, 1,);
	cr_assert(gequal(ellsea(curve.curve, 0), curve.order),);
}

Test(gp, test_gp_generators) {
	curve_t curve = {.field = stoi(19), .a = mkintmodu(3, 19), .b = mkintmodu(6, 19),
		.curve = ellinit(mkvec2(stoi(3), stoi(6)), stoi(19), 0), .order = stoi(16)
	};
	config_t cfg = {};
	arg_t arg = {.args = "(seed, field, a, b, curve, order) -> { return(ellgenerators(curve)); }", .nargs = 1};

	int ret = gp_gen(&curve, &cfg, &arg, OFFSET_GENERATORS);
	cr_assert_eq(ret, 1,);

	GEN ellgens = ellgenerators(curve.curve);
	long len = glength(ellgens);
	cr_assert_eq(len, curve.ngens,);
	for (long i = 1; i <= len; ++i) {
		cr_assert(gequal(gel(ellgens, i), curve.generators[i - 1]->point),);
	}
}

Test(gp, test_gp_points) {
	curve_t curve = {.field = stoi(19), .a = mkintmodu(3, 19), .b = mkintmodu(6, 19),
		.curve = ellinit(mkvec2(stoi(3), stoi(6)), stoi(19), 0), .order = stoi(16),
	};
	point_t gen = {.point = mkvec2(mkintmodu(4,19), mkintmodu(14,19))};
	point_t *generators[1] = {&gen};
	curve.generators = generators;
	curve.ngens = 1;

	config_t cfg = {};
	arg_t arg = {.args = "(seed, field, a, b, curve, order, gens) -> { return([ellmul(curve,gens[1],2)]); }", .nargs = 1};

	int ret = gp_gen(&curve, &cfg, &arg, OFFSET_POINTS);
	cr_assert_eq(ret, 1,);
	cr_assert_eq(curve.npoints, 1,);
	cr_assert(gequal(curve.points[0]->point, ellmul(curve.curve, gen.point, stoi(2))),);
}