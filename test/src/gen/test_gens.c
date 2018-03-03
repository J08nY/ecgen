/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include <criterion/criterion.h>
#include "exhaustive/arg.h"
#include "gen/gens.h"
#include "test/io.h"

TestSuite(gens, .init = io_setup, .fini = io_teardown);

Test(gens, test_gens_gen_any) {
	cfg->field = FIELD_PRIME;
	curve_t curve = {.field = stoi(19),
	                 .a = mkintmodu(3, 19),
	                 .b = mkintmodu(5, 19),
	                 .curve = ellinit(mkvec2(stoi(3), stoi(5)), stoi(19), 0),
	                 .order = stoi(26)};

	pari_sp to = avma;
	int ret = gens_gen_any(&curve, NULL, OFFSET_GENERATORS);
	pari_sp from = avma;
	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.generators, );
	cr_assert_eq(curve.ngens, 1, );
	gens_unroll(&curve, from, to);

	memset(&curve, 0, sizeof(curve_t));

	curve.field = stoi(29);
	curve.a = mkintmodu(5, 29);
	curve.b = mkintmodu(1, 29);
	curve.curve = ellinit(mkvec2(stoi(5), stoi(1)), stoi(29), 0);
	curve.order = stoi(32);

	ret = gens_gen_any(&curve, NULL, OFFSET_GENERATORS);
	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.generators, );
	cr_assert_eq(curve.ngens, 2, );
}

Test(gens, test_gens_gen_one) {
	cfg->field = FIELD_PRIME;
	curve_t curve = {.field = stoi(19),
	                 .a = mkintmodu(3, 19),
	                 .b = mkintmodu(5, 19),
	                 .curve = ellinit(mkvec2(stoi(3), stoi(5)), stoi(19), 0),
	                 .order = stoi(26)};

	pari_sp to = avma;
	int ret = gens_gen_one(&curve, NULL, OFFSET_GENERATORS);
	pari_sp from = avma;
	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.generators, );
	cr_assert_eq(curve.ngens, 1, );
	gens_unroll(&curve, from, to);

	memset(&curve, 0, sizeof(curve_t));

	curve.field = stoi(29);
	curve.a = mkintmodu(5, 29);
	curve.b = mkintmodu(1, 29);
	curve.curve = ellinit(mkvec2(stoi(5), stoi(1)), stoi(29), 0);
	curve.order = stoi(32);

	ret = gens_gen_one(&curve, NULL, OFFSET_GENERATORS);
	cr_assert_eq(ret, -5, );
	cr_assert_null(curve.generators, );
}

Test(gens, test_gens_check_anomalous) {
	cfg->field = FIELD_PRIME;
	curve_t curve = {
	    .field = stoi(19),
	    .a = mkintmodu(3, 19),
	    .b = mkintmodu(6, 19),
	    .curve = ellinit(mkvec2(stoi(3), stoi(6)), stoi(19), 0),
	    .order = stoi(16),
	};
	point_t gen = {.point = mkvec2(mkintmodu(4, 19), mkintmodu(14, 19)),
	               .order = stoi(16)};
	subgroup_t gen_subgroup = {.generator = &gen};
	subgroup_t *generators[1] = {&gen_subgroup};
	curve.generators = generators;
	curve.ngens = 1;

	int ret = gens_check_anomalous(&curve, NULL, OFFSET_GENERATORS);
	cr_assert_eq(ret, 1, );

	memset(&curve, 0, sizeof(curve_t));

	curve.field = stoi(23);
	curve.a = mkintmodu(5, 23);
	curve.b = mkintmodu(14, 23);
	curve.curve = ellinit(mkvec2(stoi(5), stoi(14)), stoi(23), 0);
	curve.order = stoi(23);
	gen.point = mkvec2(mkintmodu(20, 23), mkintmodu(8, 23));
	gen.order = stoi(23);
	gen.cofactor = stoi(1);
	curve.generators = generators;
	curve.ngens = 1;

	ret = gens_check_anomalous(&curve, NULL, OFFSET_GENERATORS);
	cr_assert_eq(ret, -5, );
}

Test(gens, test_gens_check_embedding) {
	cfg->field = FIELD_PRIME;
	curve_t curve = {
	    .field = stoi(19),
	    .a = mkintmodu(3, 19),
	    .b = mkintmodu(12, 19),
	    .curve = ellinit(mkvec2(stoi(3), stoi(12)), stoi(19), 0),
	    .order = stoi(18),
	};
	point_t gen1 = {.point = mkvec2(mkintmodu(1, 19), mkintmodu(4, 19)),
	                .order = stoi(6)};
	point_t gen2 = {.point = mkvec2(mkintmodu(10, 19), mkintmodu(4, 19)),
	                .order = stoi(3)};
	subgroup_t gen1_subgroup = {.generator = &gen1};
	subgroup_t gen2_subgroup = {.generator = &gen2};
	subgroup_t *generators[2] = {&gen1_subgroup, &gen2_subgroup};
	curve.generators = generators;
	curve.ngens = 2;

	char *min_degree = "5";
	arg_t arg = {.args = min_degree, .nargs = 1};

	int ret = gens_check_embedding(&curve, &arg, OFFSET_GENERATORS);
	cr_assert_eq(ret, -5, );
}