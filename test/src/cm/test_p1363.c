/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <criterion/criterion.h>
#include "cm/p1363.h"
#include "test/default.h"
#include "test/output.h"

void p1363_setup() {
	default_setup();
	output_setup();
}

void p1363_teardown() {
	default_teardown();
	output_teardown();
}

TestSuite(p1363, .init = p1363_setup, .fini = p1363_teardown);

Test(p1363, test_p1363_forms) {
	GEN D = stoi(71);
	p1363_form_t **forms;
	size_t nforms = p1363_forms(D, &forms);

	p1363_form_t expected[7] = {
	    {gen_1, gen_0, stoi(71)},     {stoi(3), gen_1, stoi(24)},
	    {stoi(3), gen_m1, stoi(24)},  {stoi(8), gen_1, stoi(9)},
	    {stoi(8), gen_m1, stoi(9)},   {stoi(5), stoi(2), stoi(15)},
	    {stoi(5), stoi(-2), stoi(15)}};
	cr_assert_eq(nforms, 7, );
	for (size_t i = 0; i < nforms; ++i) {
		cr_assert(equalii(forms[i]->A, expected[i].A), );
		cr_assert(equalii(forms[i]->B, expected[i].B), );
		cr_assert(equalii(forms[i]->C, expected[i].C), );
	}
	p1363_free(&forms, nforms);
}

Test(p1363, test_p1363_poly) {
	GEN D = stoi(71);
	p1363_form_t **forms;
	size_t nforms = p1363_forms(D, &forms);
	GEN WD = p1363_poly(D, forms, nforms);
	GEN coeffs = gtovec(WD);

	GEN expected =
	    mkvecn(8, gen_1, gen_m2, gen_m1, gen_1, gen_1, gen_1, gen_m1, gen_m1);
	cr_assert(gequal(coeffs, expected), );
	p1363_free(&forms, nforms);
}

Test(p1363, test_p1363_polclass) {
	GEN WD = p1363_polclass(stoi(71));

	GEN coeffs = gtovec(WD);

	GEN expected =
	    mkvecn(8, gen_1, gen_m2, gen_m1, gen_1, gen_1, gen_1, gen_m1, gen_m1);
	cr_assert(gequal(coeffs, expected), );
}