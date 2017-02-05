/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "field.h"


GEN field_primer(long bits) {
	return random_prime(bits);
}


GEN field_binaryr(long bits) {
	if (poly_exists(bits)) {
		return poly_find_gen(bits);
	} else {
		fprintf(stderr, "Unable to find a suitable binary field. Use an explicit one.");
		exit(1);
	}
}

GEN field_params(GEN field) {
	pari_sp ltop = avma;

	long l2;
	if (typ(field) == t_INT) {
		GEN p3 = cgetg(2, t_VEC);
		gel(p3, 1) = gcopy(field);
		p3 = gerepilecopy(ltop, p3);
		return p3;
	}

	GEN out = gtovec0(gen_0, 3);

	long j = 1;
	l2 = glength(member_mod(field)) - 2;
	{
		pari_sp btop = avma;
		for (GEN i = gen_1; gcmpgs(i, l2) <= 0; i = gaddgs(i, 1)) {
			GEN c = polcoeff0(member_mod(field), gtos(i), -1);
			if (cmpis(c, 0) != 0) {
				gel(out, j) = gcopy(i);
				j++;
			}
			if (gc_needed(btop, 1)) gerepileall(btop, 4, &out, &c, &i);
		}
	}
	out = gerepilecopy(ltop, out);
	return out;
}

GEN field_elementi(GEN element) {
	switch (typ(element)) {
		case t_INT:
			return element;
		case t_INTMOD:
			return lift(element);
		case t_FFELT: {
			pari_sp ltop = avma;
			GEN coeffs = FF_to_FpXQ(element);
			GEN vec = gtovec(coeffs);
			GEN n = fromdigits(vec, stoi(2));
			return gerepilecopy(ltop, n);
		}
		default:
			pari_err_TYPE("field_elementi", element);
			return NULL; /* NOT REACHABLE */
	}
}