/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */

#include <criterion/criterion.h>
#include <criterion/parameterized.h>
#include "exhaustive/brainpool.h"
#include "exhaustive/brainpool_rfc.h"
#include "exhaustive/check.h"
#include "exhaustive/exhaustive.h"
#include "gen/curve.h"
#include "gen/gens.h"
#include "gen/order.h"
#include "gen/seed.h"
#include "misc/types.h"
#include "test/io.h"
#include "test/memory.h"
#include "util/bits.h"
#include "util/memory.h"

TestSuite(brainpool_rfc, .init = io_setup, .fini = io_teardown);

Test(brainpool_rfc, test_brainpool_rfc_seed_random) {
	curve_t curve = {0};
	cfg->bits = 256;
	int ret = brainpool_rfc_gen_seed_random(&curve, NULL, OFFSET_SEED);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );

	seed_free(&curve.seed);
}

Test(brainpool_rfc, test_brainpool_rfc_seed_argument) {
	curve_t curve = {0};
	char *seed = "abcdefabcdefabcdefabcdefabcdefabcdefabcd";
	cfg->seed = seed;
	cfg->bits = 256;
	int ret = brainpool_rfc_gen_seed_argument(&curve, NULL, OFFSET_SEED);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );
	char *hex = bits_to_hex(curve.seed->seed);
	cr_assert_str_eq(hex, seed, );

	try_free(hex);
	seed_free(&curve.seed);
}

Test(brainpool_rfc, test_brainpool_rfc_seed_input) {
	curve_t curve = {0};
	char *seed = "abcdefabcdefabcdefabcdefabcdefabcdefabcd";
	cfg->bits = 256;
	fprintf(write_in, "%s\n", seed);
	int ret = brainpool_rfc_gen_seed_input(&curve, NULL, OFFSET_SEED);

	cr_assert_eq(ret, 1, );
	cr_assert_not_null(curve.seed, );
	char *hex = bits_to_hex(curve.seed->seed);
	cr_assert_str_eq(hex, seed, );

	try_free(hex);
	seed_free(&curve.seed);
}

struct rfc_params {
	size_t bits;
	char *p_seed;
	char *p;
	char *ab_seed;
	char *a;
	char *b;
	char *mult;
};

void params_cleanup(struct criterion_test_params *ctp) {
	struct rfc_params *params = (struct rfc_params *)ctp->params;
	cr_free(params->p_seed);
	cr_free(params->p);
	cr_free(params->ab_seed);
	cr_free(params->a);
	cr_free(params->b);
	cr_free(params->mult);
}

/**
 * These fail because brainpool_rfc_gen_equation only implements the one curve
 * generation step and
 * does not do other Brainpool checks(which are a part of the procedure).*/
ParameterizedTestParameters(brainpool_rfc, test_brainpool_rfc_params) {
	static struct rfc_params params[7] = {{0}};

	params[0].bits = 160;
	params[0].p_seed = cr_strdup("3243F6A8885A308D313198A2E03707344A409382");
	params[0].p = cr_strdup("E95E4A5F737059DC60DFC7AD95B3D8139515620F");
	params[0].ab_seed = cr_strdup("2B7E151628AED2A6ABF7158809CF4F3C762E7160");
	params[0].a = cr_strdup("340E7BE2A280EB74E2BE61BADA745D97E8F7C300");
	params[0].b = cr_strdup("1E589A8595423412134FAA2DBDEC95C8D8675E58");
	params[0].mult = cr_strdup("2187040EA6E6EC5D867AB235A349A55BAA5E9C32");
	params[1].bits = 192;
	params[1].p_seed = cr_strdup("2299F31D0082EFA98EC4E6C89452821E638D0137");
	params[1].p = cr_strdup("C302F41D932A36CDA7A3463093D18DB78FCE476DE1A86297");
	params[1].ab_seed = cr_strdup("F38B4DA56A784D9045190CFEF324E7738926CFBE");
	params[1].a = cr_strdup("6A91174076B1E0E19C39C031FE8685C1CAE040E5C69A28EF");
	params[1].b = cr_strdup("469A28EF7C28CCA3DC721D044F4496BCCA7EF4146FBF25C9");
	params[1].mult =
	    cr_strdup("6FBF25C9A6392E5353EB6D02255D716E4043DA7816C55490");
	params[2].bits = 224;
	params[2].p_seed = cr_strdup("7BE5466CF34E90C6CC0AC29B7C97C50DD3F84D5B");
	params[2].p =
	    cr_strdup("D7C134AA264366862A18302575D1D787B09F075797DA89F57EC8C0FF");
	params[2].ab_seed = cr_strdup("5F4BF8D8D8C31D763DA06C80ABB1185EB4F7C7B5");
	params[2].a =
	    cr_strdup("68A5E62CA9CE6C1C299803A6C1530B514E182AD8B0042A59CAD29F43");
	params[2].b =
	    cr_strdup("2580F63CCFE44138870713B1A92369E33E2135D266DBB372386C400B");
	params[2].mult =
	    cr_strdup("66DBB372386C400BE646C1B80C4A40580359B836DFD41B5485953527");
	params[3].bits = 256;
	params[3].p_seed = cr_strdup("5B54709179216D5D98979FB1BD1310BA698DFB5A");
	params[3].p = cr_strdup(
	    "A9FB57DBA1EEA9BC3E660A909D838D726E3BF623D52620282013481D1F6E5377");
	params[3].ab_seed = cr_strdup("757F5958490CFD47D7C19BB42158D9554F7B46BC");
	params[3].a = cr_strdup(
	    "7D5A0975FC2C3057EEF67530417AFFE7FB8055C126DC5C6CE94A4B44F330B5D9");
	params[3].b = cr_strdup(
	    "26DC5C6CE94A4B44F330B5D9BBD77CBF958416295CF7E1CE6BCCDC18FF8C07B6");
	params[3].mult = cr_strdup(
	    "5CF7E1CE6BCCDC18FF8C07B6E9B89F067C39996241690B7C6FF4A4CF27CE38F7");
	params[4].bits = 320;
	params[4].p_seed = cr_strdup("C2FFD72DBD01ADFB7B8E1AFED6A267E96BA7C904");
	params[4].p = cr_strdup(
	    "D35E472036BC4FB7E13C785ED201E065F98FCFA6F6F40DEF4F92B9EC7893EC28FCD412"
	    "B1F1B32E27");
	params[4].ab_seed = cr_strdup("ED55C4D79FD5F24D6613C31C3839A2DDF8A9A276");
	params[4].a = cr_strdup(
	    "3EE30B568FBAB0F883CCEBD46D3F3BB8A2A73513F5EB79DA66190EB085FFA9F492F375"
	    "A97D860EB4");
	params[4].b = cr_strdup(
	    "520883949DFDBC42D3AD198640688A6FE13F41349554B49ACC31DCCD884539816F5EB4"
	    "AC8FB1F1A6");
	params[4].mult = cr_strdup(
	    "1554B49ACC31DCCD884539816F5EB4AC8FB1F1A6D41318159E53765CD93C0457DF9FAF"
	    "9A000C6538");
	params[5].bits = 384;
	params[5].p_seed = cr_strdup("5F12C7F9924A19947B3916CF70801F2E2858EFC1");
	params[5].p = cr_strdup(
	    "8CB91E82A3386D280F5D6F7E50E641DF152F7109ED5456B412B1DA197FB71123ACD3A7"
	    "29901D1A71874700133107EC53");
	params[5].ab_seed = cr_strdup("BCFBFA1C877C56284DAB79CD4C2B3293D20E9E5E");
	params[5].a = cr_strdup(
	    "7BC382C63D8C150C3C72080ACE05AFA0C2BEA28E4FB22787139165EFBA91F90F8AA581"
	    "4A503AD4EB04A8C7DD22CE2826");
	params[5].b = cr_strdup(
	    "04A8C7DD22CE28268B39B55416F0447C2FB77DE107DCD2A62E880EA53EEB62D57CB439"
	    "0295DBC9943AB78696FA504C11");
	params[5].mult = cr_strdup(
	    "7DCD2A62E880EA53EEB62D57CB4390295DBC9943AB78696FA504C115037CD644E494DC"
	    "C245B3B8813113DD705F4C2C3");
	params[6].bits = 512;
	params[6].p_seed = cr_strdup("6636920D871574E69A458FEA3F4933D7E0D95748");
	params[6].p = cr_strdup(
	    "AADD9DB8DBE9C48B3FD4E6AE33C9FC07CB308DB3B3C9D20ED6639CCA703308717D4D9B"
	    "009BC66842AECDA12AE6A380E62881FF2F2D82C68528AA6056583A48F3");
	params[6].ab_seed = cr_strdup("AF02AC60ACC93ED874422A52ECB238FEEE5AB6AD");
	params[6].a = cr_strdup(
	    "7830A3318B603B89E2327145AC234CC594CBDD8D3DF91610A83441CAEA9863BC2DED5D"
	    "5AA8253AA10A2EF1C98B9AC8B57F1117A72BF2C7B9E7C1AC4D77FC94CA");
	params[6].b = cr_strdup(
	    "3DF91610A83441CAEA9863BC2DED5D5AA8253AA10A2EF1C98B9AC8B57F1117A72BF2C7"
	    "B9E7C1AC4D77FC94CADC083E67984050B75EBAE5DD2809BD638016F723");
	params[6].mult = cr_strdup(
	    "A2EF1C98B9AC8B57F1117A72BF2C7B9E7C1AC4D77FC94CADC083E67984050B75EBAE5D"
	    "D2809BD638016F723707F59380B759E9BCE57ACFDA9CB96AC38A433A6");

	size_t nb_params = sizeof(params) / sizeof(struct rfc_params);
	return cr_make_param_array(struct rfc_params, params, nb_params,
	                           params_cleanup);
}
ParameterizedTest(struct rfc_params *param, brainpool_rfc,
                  test_brainpool_rfc_params) {
	cfg->bits = param->bits;
	cfg->field = FIELD_PRIME;

	bits_t *p_bits = bits_from_hex(param->p);
	bits_t *a_bits = bits_from_hex(param->a);
	bits_t *b_bits = bits_from_hex(param->b);
	bits_t *mult_bits = bits_from_hex(param->mult);
	GEN p = bits_to_i(p_bits);
	GEN a = gmodulo(bits_to_i(a_bits), p);
	GEN b = gmodulo(bits_to_i(b_bits), p);
	GEN mult = bits_to_i(mult_bits);
	bits_free(&p_bits);
	bits_free(&a_bits);
	bits_free(&b_bits);
	bits_free(&mult_bits);

	curve_t curve = {0};
	cfg->seed = param->p_seed;
	int ret = brainpool_rfc_gen_seed_argument(&curve, NULL, OFFSET_SEED);
	cr_assert_not_null(curve.seed, );
	cr_assert_eq(ret, 1, );

	ret = brainpool_gen_field(&curve, NULL, OFFSET_FIELD);
	cr_assert_not_null(curve.field, );
	cr_assert_eq(ret, 1, );
	cr_assert(equalii(curve.field, p), );

	seed_free(&curve.seed);
	cfg->seed = param->ab_seed;
	ret = brainpool_rfc_gen_seed_argument(&curve, NULL, OFFSET_SEED);
	cr_assert_not_null(curve.seed, );
	cr_assert_eq(ret, 1, );

	exhaustive_t setup = {0};
	gen_f gens[OFFSET_END] = {0};
	gens[OFFSET_A] = &gen_skip;
	gens[OFFSET_B] = &brainpool_rfc_gen_equation;
	gens[OFFSET_CURVE] = &curve_gen_any;
	gens[OFFSET_ORDER] = &order_gen_prime;
	gens[OFFSET_GENERATORS] = &brainpool_gen_gens;
	gens[OFFSET_POINTS] = &gen_skip;
	check_t *checks[OFFSET_END] = {0};
	checks[OFFSET_ORDER] = check_new(brainpool_check_order, NULL);
	checks[OFFSET_GENERATORS] =
	    check_new(gens_check_anomalous, brainpool_check_gens, NULL);
	unroll_f unrolls[OFFSET_END] = {0};
	unrolls[OFFSET_CURVE] = &curve_unroll;
	unrolls[OFFSET_GENERATORS] = &gens_unroll;
	setup.generators = gens;
	setup.validators = checks;
	setup.unrolls = unrolls;

	ret = exhaustive_gen(&curve, &setup, OFFSET_A, OFFSET_POINTS);

	cr_assert_not_null(curve.a, );
	cr_assert_not_null(curve.b, );
	cr_assert_eq(ret, 1, );
	cr_assert(gequal(curve.a, a), );
	cr_assert(gequal(curve.b, b), );
	cr_assert(gequal(curve.seed->brainpool.mult, mult), );

	seed_free(&curve.seed);
}