/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <criterion/criterion.h>
#include "test/default.h"
#include "cm/p1363.h"


TestSuite(p1363, .init = default_setup, .fini = default_teardown);

Test(p1363, test_p1363_forms) {
	GEN D = stoi(71);
	p1363_form_t **forms;
	size_t nforms = p1363_forms(D, &forms);

	p1363_form_t expected[7] = {
			{gen_1,   gen_0,    stoi(71)},
			{stoi(3), gen_1,    stoi(24)},
			{stoi(3), gen_m1,   stoi(24)},
			{stoi(8), gen_1,    stoi(9)},
			{stoi(8), gen_m1,   stoi(9)},
			{stoi(5), stoi(2),  stoi(15)},
			{stoi(5), stoi(-2), stoi(15)}
	};
	cr_assert_eq(nforms, 7,);
	for (size_t i = 0; i < nforms; ++i) {
		cr_assert(equalii(forms[i]->A, expected[i].A), );
		cr_assert(equalii(forms[i]->B, expected[i].B), );
		cr_assert(equalii(forms[i]->C, expected[i].C), );
	}
}