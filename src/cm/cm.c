/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "cm.h"
#include "custom.h"
#include "io/output.h"
#include "obj/curve.h"
#include "p1363.h"

int cm_do() {
	debug_log_start("Starting Complex Multiplication method");

	int result = 0;
	curve_t *curve = custom_curve();
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
