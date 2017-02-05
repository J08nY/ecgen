/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "curve.h"

GEN curve_random(GEN field) {
	pari_sp ltop = avma;

	GEN curve;
	do {
		GEN a = genrand(field);
		GEN b = genrand(field);

		GEN v = gen_0;
		switch (typ(field)) {
			case t_INT:
				v = gtovec0(gen_0, 2);
				gel(v, 1) = a;
				gel(v, 2) = b;
				break;
			case t_FFELT:
				v = gtovec0(gen_0, 5);
				gel(v, 1) = gen_1;
				gel(v, 4) = a;
				gel(v, 5) = b;
				break;
			default:
				pari_err_TYPE("curve_random", field);
		}
		curve = ellinit(v, field, -1);
	} while (gequal0(ell_get_disc(curve)));

	return gerepilecopy(ltop, curve);
}
