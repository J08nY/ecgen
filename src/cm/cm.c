/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "cm.h"
#include "io/output.h"
#include "custom.h"

int cm_do() {
	debug_log_start("Starting Complex Multiplication method");

	fprintf(err, "This is *NOT IMPLEMENTED* currently.\n");

	/*
	GEN D = stoi(71);
	p1363_form_t **forms;
	size_t nforms = p1363_forms(D, &forms);
	for (size_t i = 0; i < nforms; ++i) {
		p1363_invariant(D, forms[i]);
	}
	p1363_free(&forms, nforms);
	*/
	custom_curve();

	debug_log_start("Finished Complex Multiplication method");
	return 0;
}
