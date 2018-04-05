/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "cm.h"
#include "custom.h"
#include "io/output.h"
#include "p1363.h"

int cm_do() {
	debug_log_start("Starting Complex Multiplication method");

	fprintf(err, "This is *NOT IMPLEMENTED* currently.\n");

	GEN D = stoi(71);
	p1363_form_t **forms;
	size_t nforms = p1363_forms(D, &forms);
	GEN WD = p1363_poly(D, forms, nforms);
	pari_printf("%Ps\n", WD);
	p1363_free(&forms, nforms);

	debug_log_start("Finished Complex Multiplication method");
	return 0;
}
