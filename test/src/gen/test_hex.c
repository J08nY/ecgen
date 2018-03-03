/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include <criterion/criterion.h>
#include "gen/hex.h"
#include "test/default.h"
#include "util/bits.h"

TestSuite(hex, .init = default_setup, .fini = default_teardown);

Test(hex, test_hex_seed) {
	cfg->field = FIELD_PRIME;
	char *what = "ABCDE";
	seed_t seed = {.seed = bits_from_hex(what)};
	curve_t curve = {.seed = &seed};

	arg_t arg = {.args = what, .nargs = 1};

	int ret = hex_check_param(&curve, &arg, OFFSET_SEED);
	cr_assert_eq(ret, 1, );
}

Test(hex, test_hex_field) {
	cfg->field = FIELD_PRIME;
	char *where = "0xF235ABCDE6F";
	char *what = "ABCDE";
	seed_t seed = {.seed = bits_from_hex("12345")};
	curve_t curve = {.seed = &seed, .field = strtoi(where)};

	arg_t arg = {.args = what, .nargs = 1};

	int ret = hex_check_param(&curve, &arg, OFFSET_FIELD);
	cr_assert_eq(ret, 1, );
}

Test(hex, test_hex_a) {
	cfg->field = FIELD_PRIME;
	char *where = "0xF235ABCDE6F";
	char *field = "0xFFFFFFFFFFFF";
	char *what = "ABCDE";
	seed_t seed = {.seed = bits_from_hex("12345")};
	curve_t curve = {.seed = &seed,
	                 .field = strtoi(field),
	                 .a = mkintmod(strtoi(where), strtoi(field))};

	arg_t arg = {.args = what, .nargs = 1};

	int ret = hex_check_param(&curve, &arg, OFFSET_A);
	cr_assert_eq(ret, 1, );
}

Test(hex, test_hex_b) {
	cfg->field = FIELD_PRIME;
	char *where = "0xF235ABCDE6F";
	char *field = "0xFFFFFFFFFFFF";
	char *what = "ABCDE";
	seed_t seed = {.seed = bits_from_hex("12345")};
	curve_t curve = {.seed = &seed,
	                 .field = strtoi(field),
	                 .a = mkintmod(stoi(15), strtoi(field)),
	                 .b = mkintmod(strtoi(where), strtoi(field))};

	arg_t arg = {.args = what, .nargs = 1};

	int ret = hex_check_param(&curve, &arg, OFFSET_B);
	cr_assert_eq(ret, 1, );
}

Test(hex, test_hex_order) {
	cfg->field = FIELD_PRIME;
	char *where = "0xF235ABCDE6F";
	char *what = "ABCDE";
	seed_t seed = {.seed = bits_from_hex("12345")};
	curve_t curve = {.seed = &seed,
	                 .field = stoi(21),
	                 .a = mkintmod(stoi(15), stoi(21)),
	                 .b = mkintmod(stoi(20), stoi(21)),
	                 .order = strtoi(where)};

	arg_t arg = {.args = what, .nargs = 1};

	int ret = hex_check_param(&curve, &arg, OFFSET_ORDER);
	cr_assert_eq(ret, 1, );
}

Test(hex, test_hex_generators) {
	cfg->field = FIELD_PRIME;
	char *where = "0xF235ABCDE6F";
	char *field = "0xFFFFFFFFFFFF";
	char *what = "ABCDE";
	point_t gen = {.point = mkvec2(mkintmod(strtoi(where), strtoi(field)),
	                               mkintmod(stoi(52), strtoi(field)))};
	subgroup_t gen_subgroup = {.generator = &gen};
	subgroup_t *subgroups[1] = {&gen_subgroup};

	seed_t seed = {.seed = bits_from_hex("12345")};
	curve_t curve = {.seed = &seed,
	                 .field = strtoi(field),
	                 .a = mkintmod(stoi(15), strtoi(field)),
	                 .b = mkintmod(stoi(20), strtoi(field)),
	                 .order = stoi(22),
	                 .generators = subgroups,
	                 .ngens = 1};

	arg_t arg = {.args = what, .nargs = 1};

	int ret = hex_check_param(&curve, &arg, OFFSET_GENERATORS);
	cr_assert_eq(ret, 1, );
}

Test(hex, test_hex_points) {
	cfg->field = FIELD_PRIME;
	char *where = "0xF235ABCDE6F";
	char *field = "0xFFFFFFFFFFFF";
	char *what = "ABCDE";
	point_t gen = {.point = mkvec2(mkintmod(stoi(23), strtoi(field)),
	                               mkintmod(stoi(52), strtoi(field)))};
	subgroup_t gen_subgroup = {.generator = &gen};
	subgroup_t *subgroups[1] = {&gen_subgroup};
	point_t point = {.point = mkvec2(mkintmod(strtoi(where), strtoi(field)),
	                                 mkintmod(stoi(31), strtoi(field)))};
	point_t *points[1] = {&point};
	gen_subgroup.npoints = 1;
	gen_subgroup.points = points;

	seed_t seed = {.seed = bits_from_hex("12345")};
	curve_t curve = {.seed = &seed,
	                 .field = strtoi(field),
	                 .a = mkintmod(stoi(15), strtoi(field)),
	                 .b = mkintmod(stoi(20), strtoi(field)),
	                 .order = stoi(22),
	                 .generators = subgroups,
	                 .ngens = 1};

	arg_t arg = {.args = what, .nargs = 1};

	int ret = hex_check_param(&curve, &arg, OFFSET_POINTS);
	cr_assert_eq(ret, 1, );
}