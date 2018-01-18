/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "twists.h"

void curve_twist_rand(curve_t *to, const curve_t *of) {
	to->field = gcopy(of->field);
	GEN v = elltwist(of->curve, NULL);
	to->curve = ellinit(v, to->field, -1);
	if (typ(of->field) == t_INT) {
		to->a = ell_get_a4(to->curve);
		to->b = ell_get_a6(to->curve);
	} else if (typ(of->field) == t_FFELT) {
		to->a = ell_get_a2(to->curve);
		to->b = ell_get_a6(to->curve);
	}
}