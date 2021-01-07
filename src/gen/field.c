/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "field.h"
#include "io/input.h"
#include "io/output.h"
#include "math/poly.h"
#include "misc/compat.h"

static GEN field_primer(unsigned long bits) { return random_prime(bits); }

static GEN field_binaryr(unsigned long bits) {
	if (poly_exists(bits)) {
		return poly_find_gen(bits);
	} else {
		fprintf(err,
		        "Unable to find a suitable binary field. Use an explicit one.");
		exit(1);
	}
}

GENERATOR(field_gen_random) {
	switch (cfg->field) {
		case FIELD_PRIME:
			curve->field = field_primer(cfg->bits);
			return 1;
		case FIELD_BINARY:
			curve->field = field_binaryr(cfg->bits);
			return 1;
		default:
			return INT_MIN; /* NOT REACHABLE */
	}
}

GENERATOR(field_gen_input) {
	pari_sp ltop = avma;
	switch (cfg->field) {
		case FIELD_PRIME: {
			GEN p = input_prime("p:", cfg->bits);
			if (gequalm1(p)) {
				avma = ltop;
				return 0;
			} else if (equalii(p, gen_m2)) {
				avma = ltop;
				return INT_MIN;
			}
			curve->field = p;
			return 1;
		}
		case FIELD_BINARY: {
			GEN m = input_short("m:");
			if (!equalis(m, cfg->bits)) {
				avma = ltop;
				return 0;
			}

			GEN e1 = input_short("e1:");
			if (equalii(e1, gen_m1) || gcmp(e1, m) > 0) {
				avma = ltop;
				return 0;
			}
			GEN e2 = input_short("e2:");
			if (equalii(e2, gen_m1) || gcmp(e2, m) > 0) {
				avma = ltop;
				return 0;
			}
			GEN e3 = input_short("e3:");
			if (equalii(e3, gen_m1) || gcmp(e3, m) > 0) {
				avma = ltop;
				return 0;
			}

			if (isintzero(e1) && isintzero(e2) && isintzero(e3)) {
				fprintf(err, "At least one exponent must be nonzero.\n");
				avma = ltop;
				return 0;
			}

			GEN v = gtovec0(gen_0, cfg->bits + 1);
			gel(v, itos(m) + 1) = gen_1;
			if (gsigne(e1) == 1) gel(v, itos(e1) + 1) = gen_1;
			if (gsigne(e2) == 1) gel(v, itos(e2) + 1) = gen_1;
			if (gsigne(e3) == 1) gel(v, itos(e3) + 1) = gen_1;
			gel(v, 1) = gen_1;

			GEN poly = gmul(gtopolyrev(v, -1), gmodulss(1, 2));
			if (!polisirreducible(poly)) {
				fprintf(err, "Polynomial is reducible.\n");
				avma = ltop;
				return 0;
			}

			curve->field = gerepilecopy(ltop, ffgen(poly, -1));
			return 1;
		}
		default:
			return INT_MIN; /* NOT REACHABLE */
	}
}

static GEN field = NULL;
static curve_t *curve_field = NULL;

GENERATOR(field_gen_once) {
	if (field && curve_field == curve) {
		curve->field = gcopy(field);
		return 1;
	}

	int inp = field_gen_input(curve, args, state);
	if (inp > 0) {
		field = gclone(curve->field);
		curve_field = curve;
		return 1;
	} else {
		return 0;
	}
}

GEN field_params(GEN field) {
	pari_sp ltop = avma;

	if (typ(field) == t_INT) {
		return gtovec(field);
	}

	GEN out = gtovec0(gen_0, 4);

	long j = 1;
	long l2 = glength(member_mod(field)) - 1;
	{
		pari_sp btop = avma;
		for (long i = l2; i > 0; --i) {
			GEN c = polcoeff0(member_mod(field), i, -1);
			if (cmpis(c, 0) != 0) {
				gel(out, j) = stoi(i);
				j++;
			}
			gerepileall(btop, 2, &out, &c);
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

GEN field_ielement(GEN field, GEN in) {
	switch (typ(field)) {
		case t_INT:
			return Fp_to_mod(in, field);
		case t_FFELT: {
			pari_sp ltop = avma;
			GEN coeffs = digits(in, gen_2);
			GEN poly = gtopoly(coeffs, -1);
			return gerepilecopy(ltop, Fq_to_FF(poly, field));
		}
		default:
			pari_err_TYPE("field_ielement, field is unknown type. ", field);
			return gen_m1; /* NOT REACHABLE */
	}
}

void field_quit(void) {
	if (field && isclone(field)) {
		gunclone(field);
	}
}
