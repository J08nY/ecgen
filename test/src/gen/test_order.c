/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include <criterion/criterion.h>
#include <obj/obj.h>
#include "gen/order.h"
#include "test/io.h"

TestSuite(order, .init = io_setup, .fini = io_teardown);

Test(order, test_order_gen_input) {
	curve_t curve = {.field = stoi(19),
	                 .a = mkintmodu(3, 19),
	                 .b = mkintmodu(5, 19),
	                 .curve = ellinit(mkvec2(stoi(3), stoi(5)), stoi(19), 0)};
	cfg->bits = 16;

	long ord = 26;
	fprintf(write_in, "%li\n", ord);
	int ret = order_gen_input(&curve, NULL, OFFSET_ORDER);
	cr_assert_eq(ret, 1, );
	cr_assert(gequal(curve.order, stoi(ord)), );
}

Test(order, test_order_gen_any) {
	curve_t curve = {.field = stoi(19),
	                 .a = mkintmodu(3, 19),
	                 .b = mkintmodu(5, 19),
	                 .curve = ellinit(mkvec2(stoi(3), stoi(5)), stoi(19), 0)};
	cfg->bits = 16;

	int ret = order_gen_any(&curve, NULL, OFFSET_ORDER);
	cr_assert_eq(ret, 1, );
	cr_assert(gequal(curve.order, stoi(26)), );
}

Test(order, test_order_gen_sea) {
	curve_t curve = {.field = stoi(19),
	                 .a = mkintmodu(3, 19),
	                 .b = mkintmodu(5, 19),
	                 .curve = ellinit(mkvec2(stoi(3), stoi(5)), stoi(19), 0)};
	cfg->bits = 16;

	int ret = order_gen_sea(&curve, NULL, OFFSET_ORDER);
	cr_assert_eq(ret, 1, );
	cr_assert(gequal(curve.order, stoi(26)), );
}

Test(order, test_order_gen_cofactor) {
	curve_t curve = {.field = stoi(19),
	                 .a = mkintmodu(3, 19),
	                 .b = mkintmodu(5, 19),
	                 .curve = ellinit(mkvec2(stoi(3), stoi(5)), stoi(19), 0)};
	cfg->bits = 16;

	pari_ulong smallfact = 2;
	arg_t arg = {.args = &smallfact, .nargs = 1};

	int ret = order_gen_cofactor(&curve, &arg, OFFSET_ORDER);
	cr_assert_eq(ret, 1, );
	cr_assert(gequal(curve.order, stoi(26)), );
}

Test(order, test_order_gen_prime) {
	curve_t curve = {.field = stoi(19),
	                 .a = mkintmodu(1, 19),
	                 .b = mkintmodu(4, 19),
	                 .curve = ellinit(mkvec2(stoi(1), stoi(4)), stoi(19), 0)};
	cfg->bits = 16;

	int ret = order_gen_prime(&curve, NULL, OFFSET_ORDER);
	cr_assert_eq(ret, 1, );
	cr_assert(gequal(curve.order, stoi(19)), );
}

Test(order, test_order_check_pohlig_hellman) {
	curve_t curve = {.field = stoi(19),
	                 .a = mkintmodu(3, 19),
	                 .b = mkintmodu(5, 19),
	                 .curve = ellinit(mkvec2(stoi(3), stoi(5)), stoi(19), 0),
	                 .order = stoi(26)};
	cfg->bits = 16;

	char *min_factor = "12";
	arg_t arg = {.args = min_factor, .nargs = 1};

	int ret = order_check_pohlig_hellman(&curve, &arg, OFFSET_ORDER);
	cr_assert_eq(ret, 1, );
}

Test(order, test_order_check_discriminant) {
	curve_t curve = {.field = stoi(19),
	                 .a = mkintmodu(3, 19),
	                 .b = mkintmodu(5, 19),
	                 .curve = ellinit(mkvec2(stoi(3), stoi(5)), stoi(19), 0),
	                 .order = stoi(26)};
	cfg->bits = 16;
	cfg->field = FIELD_PRIME;

	char *min_disc = "9";
	arg_t arg = {.args = min_disc, .nargs = 1};

	int ret = order_check_discriminant(&curve, &arg, OFFSET_ORDER);
	cr_assert_eq(ret, 1, );

	char *nok_disc = "12";
	arg.args = nok_disc;
	ret = order_check_discriminant(&curve, &arg, OFFSET_ORDER);
	cr_assert_eq(ret, -4, );
}