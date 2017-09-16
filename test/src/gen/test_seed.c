/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <criterion/criterion.h>
#include <criterion/parameterized.h>
#include <io/config.h>
#include "gen/seed.h"
#include "test/default.h"
#include "test/input.h"
#include "test/output.h"
#include "util/bits.h"
#include "util/memory.h"

void seed_suite_setup(void) {
	default_setup();
	input_setup();
	output_setup();
}

void seed_suite_teardown(void) {
	default_teardown();
	input_teardown();
	output_teardown();
}

TestSuite(seed, .init = seed_suite_setup, .fini = seed_suite_teardown);

Test(seed, test_seed_random) {
	curve_t curve = {};
	config_t cfg = {.bits = 256};
	int ret = seed_gen_random(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );

	seed_free(&curve.seed);
}

Test(seed, test_seed_argument) {
	curve_t curve = {};
	char *seed = "abcdefabcdefabcdefabcdefabcdefabcdefabcd";
	config_t cfg = {.seed = seed, .bits = 256};
	int ret = seed_gen_argument(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );
	char *hex = bits_to_hex(curve.seed->seed);
	cr_assert_str_eq(hex, seed, );

	try_free(hex);
	seed_free(&curve.seed);
}

Test(seed, test_seed_argument_hex) {
	curve_t curve = {};
	char *seed = "0xabcdefabcdefabcdefabcdefabcdefabcdefabcd";
	config_t cfg = {.seed = seed, .bits = 256};
	int ret = seed_gen_argument(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );
	char *hex = bits_to_hex(curve.seed->seed);
	cr_assert_str_eq(hex, seed + 2, );

	try_free(hex);
	seed_free(&curve.seed);
}

Test(seed, test_seed_input) {
	curve_t curve = {};
	char *seed = "abcdefabcdefabcdefabcdefabcdefabcdefabcd";
	config_t cfg = {.bits = 256};
	fprintf(write_in, "%s\n", seed);
	int ret = seed_gen_input(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );
	char *hex = bits_to_hex(curve.seed->seed);
	cr_assert_str_eq(hex, seed, );

	try_free(hex);
	seed_free(&curve.seed);
}

Test(seed, test_seed_input_short) {
	curve_t curve = {};
	char *seed = "abcdef";
	config_t cfg = {};
	fprintf(write_in, "%s\n", seed);
	int ret = seed_gen_input(&curve, &cfg, NULL);

	cr_assert_eq(ret, 0, );
}

struct example_params {
	size_t bits;
	char *p;
	char *seed;
	char *r;
	char *a;
	char *b;
};

ParameterizedTestParameters(seed, test_seed_prime_examples) {
	static struct example_params params[] = {
		// Taken from ANSI X9.62 J.5.1 - J.5.3; p. 115 - 117
		{.bits = 192,
			.p = "fffffffffffffffffffffffffffffffeffffffffffffffff",
			.seed = "3045AE6FC8422F64ED579528D38120EAE12196D5",
		    .r = "3099D2BBBFCB2538542DCD5FB078B6EF5F3D6FE2C745DE65",
		    .a = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFC",
		    .b = "64210519E59C80E70FA7E9AB72243049FEB8DEECC146B9B1"},
		{.bits = 192,
			.p = "fffffffffffffffffffffffffffffffeffffffffffffffff",
			.seed = "31A92EE2029FD10D901B113E990710F0D21AC6B6",
			.r = "15038D1D2E1CAFEE0299F3011C1DC75B3C2A86E135DB1E6B",
			.a = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFC",
			.b = "CC22D6DFB95C6B25E49C0D6364A4E5980C393AA21668D953"},
		{.bits = 192,
			.p = "fffffffffffffffffffffffffffffffeffffffffffffffff",
			.seed = "C469684435DEB378C4B65CA9591E2A5763059A2E",
		    .r = "25191F95024D839546D9A3375639A9967D52F1373BC4EE0B",
			.a = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFC",
		    .b = "22123DC2395A05CAA7423DAECCC94760A7D462256BD56916"},
		{.bits = 239,
		    .p = "7fffffffffffffffffffffff7fffffffffff8000000000007fffffffffff",
			.seed = "E43BB460F0B80CC0C0B075798E948060F8321B7D",
			.r = "28B85EC1ECC19EFE769EB741A6D1BA29476AA5A8F2610957D6EFE78D3783",
			.a = "7FFFFFFFFFFFFFFFFFFFFFFF7FFFFFFFFFFF8000000000007FFFFFFFFFFC",
			.b = "6B016C3BDCF18941D0D654921475CA71A9DB2FB27D1D37796185C2942C0A"
		},
		{.bits = 239,
			.p = "7fffffffffffffffffffffff7fffffffffff8000000000007fffffffffff",
			.seed = "E8B4011604095303CA3B8099982BE09FCB9AE616",
			.r = "1DF491E44E7CCAF4D1EAD8A6B90DAE09E0D33F2C6CFE7A6BA76E86713D52",
			.a = "7FFFFFFFFFFFFFFFFFFFFFFF7FFFFFFFFFFF8000000000007FFFFFFFFFFC",
			.b = "617FAB6832576CBBFED50D99F0249C3FEE58B94BA0038C7AE84C8C832F2C"
		},
		{.bits = 239,
			.p = "7fffffffffffffffffffffff7fffffffffff8000000000007fffffffffff",
			.seed = "7D7374168FFE3471B60A857686A19475D3BFA2FF",
			.r = "3A4F9DC9A6CEFD5F9D1193B9C9968C202430003C2819C2E498618DC58330",
			.a = "7FFFFFFFFFFFFFFFFFFFFFFF7FFFFFFFFFFF8000000000007FFFFFFFFFFC",
			.b = "255705FA2A306654B1F4CB03D6A750A30C250102D4988717D9BA15AB6D3E"
		},
		{.bits = 256,
		    .p = "ffffffff00000001000000000000000000000000ffffffffffffffffffffffff",
			.seed = "C49D360886E704936A6678E1139D26B7819F7E90",
		    .r = "7EFBA1662985BE9403CB055C75D4F7E0CE8D84A9C5114ABCAF3177680104FA0D",
			.a = "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC",
			.b = "5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B"
		}
	};

	size_t nb_params = sizeof(params) / sizeof(struct example_params);
	return cr_make_param_array(struct example_params, params, nb_params, NULL);
}
ParameterizedTest(struct example_params *param, seed, test_seed_prime_examples) {
	//TODO: implement the test
	config_t cfg = {};
	cfg.bits = param->bits;
	curve_t curve = {};
}