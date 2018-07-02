/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "supersingular.h"

GENERATOR(supersingular_gen_equation) {
	if (equalis(curve->field, 2)) {
		return -2;
	}
	if (mod4(curve->field) == 3) {
		curve->a = mkintmod(subis(curve->field, 1), curve->field);
		curve->b = mkintmod(stoi(0), curve->field);
		return 1;
	}
	GEN q = stoi(3);
	while (!(mod4(q) == 3 && kronecker(curve->field, q) == -1)) {
		q = nextprime(addis(q, 1));
	}

	if (equalis(q, 3)) {
		curve->a = mkintmod(stoi(0), curve->field);
		curve->b = mkintmod(stoi(1), curve->field);
		return 1;
	} else {
		GEN H = polclass(negi(q), 0, 0);
		GEN r = FpX_roots(H, curve->field);
		GEN root = gel(r, 1);
		curve->a = mkintmod(
		    Fp_div(Fp_mul(stoi(27), root, curve->field),
		           Fp_mul(stoi(4), Fp_sub(stoi(1728), root, curve->field),
		                  curve->field),
		           curve->field),
		    curve->field);
		curve->b = gneg(curve->a);
		return 1;
	}
}

GENERATOR(supersingular_gen_order) {
	// copy field to order
	curve->order = addis(curve->field, 1);
	obj_insert(curve->curve, 1, curve->order);
	return 1;
}
