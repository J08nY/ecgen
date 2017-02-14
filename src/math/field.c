/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "field.h"
#include "io/input.h"
#include "poly.h"
#include "random.h"

GEN field_primer(long bits) { return random_prime(bits); }

GEN field_binaryr(long bits) {
	if (poly_exists(bits)) {
		return poly_find_gen(bits);
	} else {
		fprintf(stderr,
		        "Unable to find a suitable binary field. Use an explicit one.");
		exit(1);
	}
}

int field_random(curve_t *curve, config_t *config, ...) {
	switch (config->field) {
		case FIELD_PRIME:
			curve->field = field_primer(config->bits);
			return 1;
		case FIELD_BINARY:
			curve->field = field_binaryr(config->bits);
			return 1;
		default:
			return INT_MIN; /* NOT REACHABLE */
	}
}

int field_input(curve_t *curve, config_t *config, ...) {
	pari_sp ltop = avma;
	switch (config->field) {
		case FIELD_PRIME: {
			GEN p = fread_prime(in, "p:", config->bits);
			if (equalii(p, gen_m1)) {
				avma = ltop;
				return 0;
			}
			curve->field = p;
			return 1;
		}
		case FIELD_BINARY: {
			GEN e1 = fread_short(in, "e1:");
			if (equalii(e1, gen_m1)) {
				avma = ltop;
				return 0;
			}
			GEN e2 = fread_short(in, "e2:");
			if (equalii(e2, gen_m1)) {
				avma = ltop;
				return 0;
			}
			GEN e3 = fread_short(in, "e3:");
			if (equalii(e3, gen_m1)) {
				avma = ltop;
				return 0;
			}

			if (isintzero(e1) && isintzero(e2) && isintzero(e3)) {
				fprintf(stderr, "At least one exponent must be nonzero.\n");
				avma = ltop;
				return 0;
			}

			GEN v = gtovec0(gen_0, config->bits + 1);
			gel(v, config->bits + 1) = gen_1;
			if (gsigne(e1) == 1) gel(v, itos(e1) + 1) = gen_1;
			if (gsigne(e2) == 1) gel(v, itos(e2) + 1) = gen_1;
			if (gsigne(e3) == 1) gel(v, itos(e3) + 1) = gen_1;
			gel(v, 1) = gen_1;

			GEN poly = gmul(gtopolyrev(v, -1), gmodulss(1, 2));

			GEN field = gerepilecopy(ltop, ffgen(poly, -1));
			curve->field = field;
			return 1;
		}
		default:
			return INT_MIN; /* NOT REACHABLE */
	}
}

GEN field_params(GEN field) {
	pari_sp ltop = avma;

	if (typ(field) == t_INT) {
		return gtovec(field);
	}

	GEN out = gtovec0(gen_0, 3);

	long j = 1;
	long l2 = glength(member_mod(field)) - 2;
	{
		pari_sp btop = avma;
		for (long i = l2; i > 0; --i) {
			GEN c = polcoeff0(member_mod(field), i, -1);
			if (cmpis(c, 0) != 0) {
				gel(out, j) = stoi(i);
				j++;
			}
			if (gc_needed(btop, 1)) gerepileall(btop, 3, &out, &c);
		}
	}
	return gerepilecopy(ltop, out);
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