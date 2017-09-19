/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <criterion/criterion.h>
#include <criterion/parameterized.h>
#include "gen/types.h"
#include "math/poly.h"
#include "exhaustive/ansi.h"
#include "gen/seed.h"
#include "gen/field.h"
#include "test/default.h"
#include "test/memory.h"
#include "test/input.h"
#include "test/output.h"
#include "util/bits.h"
#include "util/memory.h"

void ansi_suite_setup(void) {
	default_setup();
	input_setup();
	output_setup();
}

void ansi_suite_teardown(void) {
	default_teardown();
	input_teardown();
	output_teardown();
}

TestSuite(ansi, .init = ansi_suite_setup, .fini = ansi_suite_teardown);

Test(ansi, test_seed_random) {
	curve_t curve = {};
	config_t cfg = {.bits = 256};
	int ret = ansi_gen_seed_random(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );

	seed_free(&curve.seed);
}

Test(ansi, test_seed_argument) {
	curve_t curve = {};
	char *seed = "abcdefabcdefabcdefabcdefabcdefabcdefabcd";
	config_t cfg = {.seed = seed, .bits = 256};
	int ret = ansi_gen_seed_argument(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );
	char *hex = bits_to_hex(curve.seed->seed);
	cr_assert_str_eq(hex, seed, );

	try_free(hex);
	seed_free(&curve.seed);
}

Test(ansi, test_seed_argument_hex) {
	curve_t curve = {};
	char *seed = "0xabcdefabcdefabcdefabcdefabcdefabcdefabcd";
	config_t cfg = {.seed = seed, .bits = 256};
	int ret = ansi_gen_seed_argument(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );
	char *hex = bits_to_hex(curve.seed->seed);
	cr_assert_str_eq(hex, seed + 2, );

	try_free(hex);
	seed_free(&curve.seed);
}

Test(ansi, test_seed_input) {
	curve_t curve = {};
	char *seed = "abcdefabcdefabcdefabcdefabcdefabcdefabcd";
	config_t cfg = {.bits = 256};
	fprintf(write_in, "%s\n", seed);
	int ret = ansi_gen_seed_input(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );
	char *hex = bits_to_hex(curve.seed->seed);
	cr_assert_str_eq(hex, seed, );

	try_free(hex);
	seed_free(&curve.seed);
}

Test(ansi, test_seed_input_short) {
	curve_t curve = {};
	char *seed = "abcdef";
	config_t cfg = {};
	fprintf(write_in, "%s\n", seed);
	int ret = ansi_gen_seed_input(&curve, &cfg, NULL);

	cr_assert_eq(ret, 0, );
}

struct prime_params {
	size_t bits;
	char *p;
	char *seed;
	char *r;
	char *a;
	char *b;
};

void prime_params_cleanup(struct criterion_test_params *ctp) {
	struct prime_params *params = (struct prime_params *)ctp->params;
	cr_free(params->p);
	cr_free(params->seed);
	cr_free(params->r);
	cr_free(params->a);
	cr_free(params->b);
}

ParameterizedTestParameters(ansi, test_seed_prime_examples) {
	static struct prime_params params[7] = {};
	// Taken from ANSI X9.62 J.5.1 - J.5.3; p. 115 - 117
	params[0].bits = 192;
	params[0].p = cr_strdup("fffffffffffffffffffffffffffffffeffffffffffffffff");
	params[0].seed = cr_strdup("3045AE6FC8422F64ED579528D38120EAE12196D5");
	params[0].r = cr_strdup("3099D2BBBFCB2538542DCD5FB078B6EF5F3D6FE2C745DE65");
	params[0].a = cr_strdup("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFC");
	params[0].b = cr_strdup("64210519E59C80E70FA7E9AB72243049FEB8DEECC146B9B1");
	params[1].bits = 192;
	params[1].p = cr_strdup("fffffffffffffffffffffffffffffffeffffffffffffffff");
	params[1].seed = cr_strdup("31A92EE2029FD10D901B113E990710F0D21AC6B6");
	params[1].r = cr_strdup("15038D1D2E1CAFEE0299F3011C1DC75B3C2A86E135DB1E6B");
	params[1].a = cr_strdup("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFC");
	params[1].b = cr_strdup("CC22D6DFB95C6B25E49C0D6364A4E5980C393AA21668D953");
	params[2].bits = 192;
	params[2].p = cr_strdup("fffffffffffffffffffffffffffffffeffffffffffffffff");
	params[2].seed = cr_strdup("C469684435DEB378C4B65CA9591E2A5763059A2E");
	params[2].r = cr_strdup("25191F95024D839546D9A3375639A9967D52F1373BC4EE0B");
	params[2].a = cr_strdup("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFC");
	params[2].b = cr_strdup("22123DC2395A05CAA7423DAECCC94760A7D462256BD56916");
	params[3].bits = 239;
	params[3].p = cr_strdup("7fffffffffffffffffffffff7fffffffffff8000000000007fffffffffff");
	params[3].seed = cr_strdup("E43BB460F0B80CC0C0B075798E948060F8321B7D");
	params[3].r = cr_strdup("28B85EC1ECC19EFE769EB741A6D1BA29476AA5A8F2610957D6EFE78D3783");
	params[3].a = cr_strdup("7FFFFFFFFFFFFFFFFFFFFFFF7FFFFFFFFFFF8000000000007FFFFFFFFFFC");
	params[3].b = cr_strdup("6B016C3BDCF18941D0D654921475CA71A9DB2FB27D1D37796185C2942C0A");
	params[4].bits = 239;
	params[4].p = cr_strdup("7fffffffffffffffffffffff7fffffffffff8000000000007fffffffffff");
	params[4].seed = cr_strdup("E8B4011604095303CA3B8099982BE09FCB9AE616");
	params[4].r = cr_strdup("1DF491E44E7CCAF4D1EAD8A6B90DAE09E0D33F2C6CFE7A6BA76E86713D52");
	params[4].a = cr_strdup("7FFFFFFFFFFFFFFFFFFFFFFF7FFFFFFFFFFF8000000000007FFFFFFFFFFC");
	params[4].b = cr_strdup("617FAB6832576CBBFED50D99F0249C3FEE58B94BA0038C7AE84C8C832F2C");
	params[5].bits = 239;
	params[5].p = cr_strdup("7fffffffffffffffffffffff7fffffffffff8000000000007fffffffffff");
	params[5].seed = cr_strdup("7D7374168FFE3471B60A857686A19475D3BFA2FF");
	params[5].r = cr_strdup("3A4F9DC9A6CEFD5F9D1193B9C9968C202430003C2819C2E498618DC58330");
	params[5].a = cr_strdup("7FFFFFFFFFFFFFFFFFFFFFFF7FFFFFFFFFFF8000000000007FFFFFFFFFFC");
	params[5].b = cr_strdup("255705FA2A306654B1F4CB03D6A750A30C250102D4988717D9BA15AB6D3E");
	params[6].bits = 256;
	params[6].p = cr_strdup("ffffffff00000001000000000000000000000000ffffffffffffffffffffffff");
	params[6].seed = cr_strdup("C49D360886E704936A6678E1139D26B7819F7E90");
	params[6].r = cr_strdup("7EFBA1662985BE9403CB055C75D4F7E0CE8D84A9C5114ABCAF3177680104FA0D");
	params[6].a = cr_strdup("FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC");
	params[6].b = cr_strdup("5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B");

	size_t nb_params = sizeof(params) / sizeof(struct prime_params);
	return cr_make_param_array(struct prime_params, params, nb_params, NULL);
}
ParameterizedTest(struct prime_params *param, ansi, test_seed_prime_examples) {
	config_t cfg = {};
	cfg.bits = param->bits;
	cfg.field = FIELD_PRIME;
	cfg.seed = param->seed;
	curve_t curve = {};
	bits_t *p = bits_from_hex(param->p);
	curve.field = bits_to_i(p);

	int ret = ansi_gen_seed_argument(&curve, &cfg, NULL);
	cr_assert_eq(ret, 1,);

	ret = ansi_gen_equation(&curve, &cfg, NULL);
	cr_assert_eq(ret, 1,);
	GEN expected_r = bits_to_i(bits_from_hex(param->r));
	cr_assert(gequal(curve.seed->ansi.r, expected_r),);

	bits_free(&p);
	seed_free(&curve.seed);
}

struct binary_params {
	size_t bits;
	polynomial_t field;
	char *seed;
	char *a;
	char *b;
};

void binary_params_cleanup(struct criterion_test_params *ctp) {
	struct binary_params *params = (struct binary_params *)ctp->params;
	cr_free(params->seed);
	cr_free(params->a);
	cr_free(params->b);
}

ParameterizedTestParameters(ansi, test_seed_binary_examples) {
	static struct binary_params params[10] = {};
	// Taken from ANSI X9.62 J.4.1, J.4.3, J.4.5 and J.4.8; p. 107 - 113
	polynomial_t p163 = {163, 9, 3, 2};
	params[0].bits = 163;
	params[0].field = p163;
	params[0].seed = cr_strdup("D2C0FB15760860DEF1EEF4D696E6768756151754");
	params[0].a = cr_strdup("072546B5435234A422E0789675F432C89435DE5242");
	params[0].b = cr_strdup("00C9517D06D5240D3CFF38C74B20B6CD4D6F9DD4D9");
	params[1].bits = 163,
	params[1].field = p163;
	params[1].seed = cr_strdup("53814C050D44D696E67687561517580CA4E29FFD");
	params[1].a = cr_strdup("0108B39E77C4B108BED981ED0E890E117C511CF072");
	params[1].b = cr_strdup("0667ACEB38AF4E488C407433FFAE4F1C811638DF20");
	params[2].bits = 163;
	params[2].field = p163;
	params[2].seed = cr_strdup("50CBF1D95CA94D696E676875615175F16A36A3B8");
	params[2].a = cr_strdup("07A526C63D3E25A256A007699F5447E32AE456B50E");
	params[2].b = cr_strdup("03F7061798EB99E238FD6F1BF95B48FEEB4854252B");
	polynomial_t p191 = {191, 10};
	params[3].bits = 191;
	params[3].field = p191;
	params[3].seed = cr_strdup("4E13CA542744D696E67687561517552F279A8C84");
	params[3].a = cr_strdup("2866537B676752636A68F56554E12640276B649EF7526267");
	params[3].b = cr_strdup("2E45EF571F00786F67B0081B9495A3D95462F5DE0AA185EC");
	params[4].bits = 191;
	params[4].field = p191;
	params[4].seed = cr_strdup("0871EF2FEF24D696E6768756151758BEE0D95C15");
	params[4].a = cr_strdup("401028774D7777C7B7666D1366EA432071274F89FF01E718");
	params[4].b = cr_strdup("0620048D28BCBD03B6249C99182B7C8CD19700C362C46A01");
	params[5].bits = 191;
	params[5].field = p191;
	params[5].seed = cr_strdup("E053512DC684D696E676875615175067AE786D1F");
	params[5].a = cr_strdup("6C01074756099122221056911C77D77E77A777E7E7E77FCB");
	params[5].b = cr_strdup("71FE1AF926CF847989EFEF8DB459F66394D90F32AD3F15E8");
	polynomial_t p239 = {239, 37};
	params[6].bits = 239;
	params[6].field = p239;
	params[6].seed = cr_strdup("D34B9A4D696E676875615175CA71B920BFEFB05D");
	params[6].a = cr_strdup("32010857077C5431123A46B808906756F543423E8D27877578125778AC76");
	params[6].b = cr_strdup("790408F2EEDAF392B012EDEFB3392F30F4327C0CA3F31FC383C422AA8C16");
	params[7].bits = 239;
	params[7].field = p239;
	params[7].seed = cr_strdup("2AA6982FDFA4D696E676875615175D266727277D");
	params[7].a = cr_strdup("4230017757A767FAE42398569B746325D45313AF0766266479B75654E65F");
	params[7].b = cr_strdup("5037EA654196CFF0CD82B2C14A2FCF2E3FF8775285B545722F03EACDB74B");
	params[8].bits = 239;
	params[8].field = p239;
	params[8].seed = cr_strdup("9E076F4D696E676875615175E11E9FDD77F92041");
	params[8].a = cr_strdup("01238774666A67766D6676F778E676B66999176666E687666D8766C66A9F");
	params[8].b = cr_strdup("6A941977BA9F6A435199ACFC51067ED587F519C5ECB541B8E44111DE1D40");
	polynomial_t p359 = {359, 69};
	params[9].bits = 359;
	params[9].field = p359;
	params[9].seed = cr_strdup("2B354920B724D696E67687561517585BA1332DC6");
	params[9].a = cr_strdup("5667676A654B20754F356EA92017D946567C46675556F19556A04616B567D223A5E05656FB549016A96656A557");
	params[9].b = cr_strdup("2472E2D0197C49363F1FE7F5B6DB075D52B6947D135D8CA445805D39BC345626089687742B6329E70680231988");

	size_t nb_params = sizeof(params) / sizeof(struct binary_params);
	return cr_make_param_array(struct binary_params, params, nb_params, binary_params_cleanup);
}
ParameterizedTest(struct binary_params *param, ansi, test_seed_binary_examples) {
	config_t cfg = {};
	cfg.bits = param->bits;
	cfg.field = FIELD_BINARY;
	cfg.seed = param->seed;
	curve_t curve = {};
	curve.field = poly_gen(&param->field);

	GEN expected_b = bits_to_i(bits_from_hex(param->b));
	bits_t *b = bits_from_i(expected_b);

	int ret = ansi_gen_seed_argument(&curve, &cfg, NULL);
	cr_assert_eq(ret, 1,);

	ret = ansi_gen_equation(&curve, &cfg, NULL);
	cr_assert_eq(ret, 1,);
	GEN curve_b = field_elementi(curve.b);
	cr_assert(gequal(curve_b, expected_b),);

	bits_free(&b);
	seed_free(&curve.seed);
}