/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "cm.h"
#include "cm_any.h"
#include "cm_prime.h"
#include "io/output.h"
#include "obj/curve.h"
#include "p1363.h"

int cm_do() {
	debug_log_start("Starting Complex Multiplication method");

	int result = 0;
	GEN order = strtoi(cfg->cm_order);
	curve_t *curve = NULL;

	if (gequal0(order)) {
		fprintf(err, "Order requested not a number: %s\n", cfg->cm_order);
		result = 1;
	} else if (isprime(order)) {
		debug_log("Starting prime order curve generation");
		curve = cm_prime_curve(order);
	} else {
		debug_log("Starting composite order curve generation");
		curve = cm_any_curve(order);
	}

	if (curve) {
		output_o_begin();
		output_o(curve);
		output_o_end();

		curve_free(&curve);
	} else {
		result = 1;
	}

	debug_log_start("Finished Complex Multiplication method");
	return result;
}
