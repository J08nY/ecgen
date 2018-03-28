/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "curve.h"
#include "math/twists.h"

GENERATOR(curve_gen_any) {
	pari_sp ltop = avma;
	GEN v = gen_0;
	switch (typ(curve->field)) {
		case t_INT:
			v = gtovec0(gen_0, 2);
			gel(v, 1) = curve->a;
			gel(v, 2) = curve->b;
			break;
		case t_FFELT:
			v = gtovec0(gen_0, 5);
			gel(v, 1) = gen_1;
			gel(v, 2) = curve->a;
			gel(v, 5) = curve->b;
			break;
		default:
			pari_err_TYPE("curve_gen_any", curve->field);
	}
	GEN crv = ellinit(v, curve->field, -1);

	if (glength(crv) == 0) {
		avma = ltop;
		return -3;
	} else {
		curve->curve = gerepilecopy(ltop, crv);
		return 1;
	}
}

GENERATOR(curve_gen_any_twist) {
	int result = curve_gen_any(curve, args, state);
	if (result != 1) {
		return result;
	} else {
		twist_rand(curve);
		return 1;
	}
}

CHECK(curve_check_nonzero) {
	if (gequal0(ell_get_disc(curve->curve))) {
		return -3;
	} else {
		return 1;
	}
}

UNROLL(curve_unroll) {
	if (curve->curve) {
		obj_free(curve->curve);
		curve->curve = NULL;
	}
	return -1;
}
