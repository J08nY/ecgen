/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include <criterion/criterion.h>
#include <misc/types.h>
#include "gen/gp.h"
#include "test/default.h"
#include "util/bits.h"

TestSuite(gp, .init = default_setup, .fini = default_teardown);

Test(gp, test_gp_gen_seed) {
	curve_t curve = {0};
	arg_t arg = {.args = "() -> { return(Vecsmall([1,0])); }", .nargs = 1};

	int ret = gp_gen(&curve, &arg, OFFSET_SEED);
	cr_assert_eq(ret, 1,);
	cr_assert_not_null(curve.seed,);
	cr_assert_not_null(curve.seed->seed,);
	cr_assert(bits_eq(curve.seed->seed, bits_from_bin("10")),);
}

Test(gp, test_gp_gen_field) {
	curve_t curve = {0};
	arg_t arg = {.args = "(seed) -> { return(19); }", .nargs = 1};

	int ret = gp_gen(&curve, &arg, OFFSET_FIELD);
	cr_assert_eq(ret, 1,);
	cr_assert(gequal(curve.field, stoi(19)),);
}

Test(gp, test_gp_gen_a) {
	curve_t curve = {.field = stoi(19)};
	arg_t arg = {.args = "(seed, field) -> { return(Mod(3,field)); }", .nargs = 1};

	int ret = gp_gen(&curve, &arg, OFFSET_A);
	cr_assert_eq(ret, 1,);
	cr_assert(gequal(curve.a, mkintmodu(3, 19)),);
}

Test(gp, test_gp_gen_b) {
	curve_t curve = {.field = stoi(19), .a = mkintmodu(3, 19)};
	arg_t arg = {.args = "(seed, field, a) -> { return(a * 2); }", .nargs = 1};

	int ret = gp_gen(&curve, &arg, OFFSET_B);
	cr_assert_eq(ret, 1,);
	cr_assert(gequal(curve.b, mkintmodu(6, 19)),);
}

Test(gp, test_gp_gen_curve) {
	curve_t curve = {.field = stoi(19), .a = mkintmodu(3, 19), .b = mkintmodu(6, 19)};
	arg_t arg = {.args = "(seed, field, a, b) -> { return(ellinit([a,b], field)); }", .nargs = 1};

	int ret = gp_gen(&curve, &arg, OFFSET_CURVE);
	cr_assert_eq(ret, 1,);
	cr_assert(gequal(curve.curve, ellinit(mkvec2(curve.a, curve.b), curve.field, 0)),);
}

Test(gp, test_gp_gen_order) {
	curve_t curve = {.field = stoi(19), .a = mkintmodu(3, 19), .b = mkintmodu(6, 19),
	.curve = ellinit(mkvec2(stoi(3), stoi(6)), stoi(19), 0)};
	arg_t arg = {.args = "(seed, field, a, b, curve) -> { return(ellsea(curve)); }", .nargs = 1};

	int ret = gp_gen(&curve, &arg, OFFSET_ORDER);
	cr_assert_eq(ret, 1,);
	cr_assert(gequal(ellsea(curve.curve, 0), curve.order),);
}

Test(gp, test_gp_gen_generators) {
	curve_t curve = {.field = stoi(19), .a = mkintmodu(3, 19), .b = mkintmodu(6, 19),
		.curve = ellinit(mkvec2(stoi(3), stoi(6)), stoi(19), 0), .order = stoi(16)
	};
	arg_t arg = {.args = "(seed, field, a, b, curve, order) -> { return(ellgenerators(curve)); }", .nargs = 1};

	int ret = gp_gen(&curve, &arg, OFFSET_GENERATORS);
	cr_assert_eq(ret, 1,);

	GEN ellgens = ellgenerators(curve.curve);
	long len = glength(ellgens);
	cr_assert_eq(len, curve.ngens,);
	for (long i = 1; i <= len; ++i) {
		cr_assert(gequal(gel(ellgens, i), curve.generators[i - 1]->point),);
	}
}

Test(gp, test_gp_gen_points) {
	curve_t curve = {.field = stoi(19), .a = mkintmodu(3, 19), .b = mkintmodu(6, 19),
		.curve = ellinit(mkvec2(stoi(3), stoi(6)), stoi(19), 0), .order = stoi(16),
	};
	point_t gen = {.point = mkvec2(mkintmodu(4,19), mkintmodu(14,19))};
	point_t *generators[1] = {&gen};
	curve.generators = generators;
	curve.ngens = 1;

	arg_t arg = {.args = "(seed, field, a, b, curve, order, gens) -> { return([ellmul(curve,gens[1],2)]); }", .nargs = 1};

	int ret = gp_gen(&curve, &arg, OFFSET_POINTS);
	cr_assert_eq(ret, 1,);
	cr_assert_eq(curve.npoints, 1,);
	cr_assert(gequal(curve.points[0]->point, ellmul(curve.curve, gen.point, stoi(2))),);
}

Test(gp, test_gp_check_seed) {
	seed_t seed = {.seed = bits_from_hex("ff")};
	curve_t curve = {.seed = &seed};

	arg_t arg = {.args="(seed) -> { return(1);}"};

	int ret = gp_check(&curve, &arg, OFFSET_SEED);
	cr_assert_eq(ret, 1,);
}

Test(gp, test_gp_check_field) {
	seed_t seed = {.seed = bits_from_hex("ff")};
	curve_t curve = {.seed = &seed, .field = stoi(19)};

	arg_t arg = {.args="(seed, field) -> { if(field == 19, return(1));}"};

	int ret = gp_check(&curve, &arg, OFFSET_FIELD);
	cr_assert_eq(ret, 1,);
}

Test(gp, test_gp_check_a) {
	seed_t seed = {.seed = bits_from_hex("ff")};
	curve_t curve = {.seed = &seed, .field = stoi(19), .a = mkintmodu(3, 19)};

	arg_t arg = {.args="(seed, field, a) -> { if(a == Mod(3,19), return(1));}"};

	int ret = gp_check(&curve, &arg, OFFSET_A);
	cr_assert_eq(ret, 1,);
}

Test(gp, test_gp_check_b) {
	seed_t seed = {.seed = bits_from_hex("ff")};
	curve_t curve = {.seed = &seed, .field = stoi(19), .a = mkintmodu(3, 19), .b = mkintmodu(5, 19)};

	arg_t arg = {.args="(seed, field, a, b) -> { if(b == Mod(5,19), return(1));}"};

	int ret = gp_check(&curve, &arg, OFFSET_B);
	cr_assert_eq(ret, 1,);
}

Test(gp, test_gp_check_curve) {
	seed_t seed = {.seed = bits_from_hex("ff")};
	curve_t curve = {.seed = &seed, .field = stoi(19), .a = mkintmodu(3, 19), .b = mkintmodu(5, 19),
		.curve = ellinit(mkvec2(stoi(3), stoi(5)), stoi(19), 0)};

	arg_t arg = {.args="(seed, field, a, b, curve) -> { if(curve == ellinit([3, 5], 19), return(1));}"};

	int ret = gp_check(&curve, &arg, OFFSET_CURVE);
	cr_assert_eq(ret, 1,);
}

Test(gp, test_gp_check_order) {
	seed_t seed = {.seed = bits_from_hex("ff")};
	curve_t curve = {.seed = &seed, .field = stoi(19), .a = mkintmodu(3, 19), .b = mkintmodu(5, 19),
		.curve = ellinit(mkvec2(stoi(3), stoi(5)), stoi(19), 0), .order = stoi(16)};

	arg_t arg = {.args="(seed, field, a, b, curve, order) -> { if(order == 16, return(1));}"};

	int ret = gp_check(&curve, &arg, OFFSET_ORDER);
	cr_assert_eq(ret, 1,);
}

Test(gp, test_gp_check_generators) {
	seed_t seed = {.seed = bits_from_hex("ff")};
	curve_t curve = {.seed = &seed, .field = stoi(19), .a = mkintmodu(3, 19), .b = mkintmodu(6, 19),
		.curve = ellinit(mkvec2(stoi(3), stoi(6)), stoi(19), 0), .order = stoi(16),
	};
	point_t gen = {.point = mkvec2(mkintmodu(4,19), mkintmodu(14,19))};
	point_t *generators[1] = {&gen};
	curve.generators = generators;
	curve.ngens = 1;

	arg_t arg = {.args="(seed, field, a, b, curve, order, gens) -> { if(gens == ellgenerators(curve), return(1));}"};

	int ret = gp_check(&curve, &arg, OFFSET_GENERATORS);
	cr_assert_eq(ret, 1,);
}

Test(gp, test_gp_check_points) {
	seed_t seed = {.seed = bits_from_hex("ff")};
	curve_t curve = {.seed = &seed, .field = stoi(19), .a = mkintmodu(3, 19), .b = mkintmodu(6, 19),
		.curve = ellinit(mkvec2(stoi(3), stoi(6)), stoi(19), 0), .order = stoi(16),
	};
	point_t gen = {.point = mkvec2(mkintmodu(4,19), mkintmodu(14,19))};
	point_t *generators[1] = {&gen};
	curve.generators = generators;
	curve.ngens = 1;
	curve.points = generators;
	curve.npoints = 1;

	arg_t arg = {.args="(seed, field, a, b, curve, order, gens, points) -> { if(points == ellgenerators(curve), return(1));}"};

	int ret = gp_check(&curve, &arg, OFFSET_POINTS);
	cr_assert_eq(ret, 1,);
}