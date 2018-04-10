/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "p1363.h"
#include "io/output.h"
#include "util/memory.h"

static GEN p1363_group(GEN D) {
	pari_sp ltop = avma;
	GEN s = mpfloor(sqrtr(rdivis(D, 3, BIGDEFAULTPREC)));
	long llen = itos(s) * 2;
	GEN l = gtovec0(gen_0, llen);

	long j = 1;
	GEN B = gen_0;
	for (; cmpii(B, s) <= 0; B = addii(B, gen_1)) {
		GEN B2 = sqri(B);
		GEN d = addii(D, B2);
		GEN lower = mulis(B, 2);
		GEN upper = gsqrt(d, BIGDEFAULTPREC);

		GEN divs = divisors(d);
		long len = glength(divs);
		for (long i = 1; i <= len; ++i) {
			GEN div = gel(divs, i);
			if (gcmp(div, lower) >= 0 && gcmp(div, upper) <= 0) {
				GEN A = div;
				GEN C = divii(d, A);
				GEN gcd = gcdii(gcdii(A, lower), C);

				if (gequal1(gcd)) {
					GEN v = gtovec0(gen_0, 3);
					gel(v, 1) = A;
					gel(v, 2) = B;
					gel(v, 3) = C;
					gel(l, j++) = v;
					if (cmpii(gen_0, lower) < 0 && cmpii(lower, A) < 0 &&
					    cmpii(A, C) < 0) {
						GEN n = gtovec0(gen_0, 3);
						gel(n, 1) = A;
						gel(n, 2) = negi(B);
						gel(n, 3) = C;
						gel(l, j++) = n;
					}
				}
			}
		}
	}
	return gerepilecopy(ltop, vec_shorten(l, j - 1));
}

static size_t p1363_num(GEN group) { return (size_t)glength(group); }

size_t p1363_forms(GEN D, p1363_form_t ***forms) {
	GEN group = p1363_group(D);
	size_t nforms = p1363_num(group);
	*forms = try_calloc(nforms * sizeof(p1363_form_t *));

	for (size_t i = 0; i < nforms; ++i) {
		(*forms)[i] = try_calloc(sizeof(p1363_form_t));
		(*forms)[i]->A = gel(gel(group, i + 1), 1);
		(*forms)[i]->B = gel(gel(group, i + 1), 2);
		(*forms)[i]->C = gel(gel(group, i + 1), 3);
	}

	return nforms;
}

void p1363_free(p1363_form_t ***forms, size_t nforms) {
	if (*forms) {
		for (size_t i = 0; i < nforms; ++i) {
			if ((*forms)[i]) {
				try_free((*forms)[i]);
				(*forms)[i] = NULL;
			}
		}
		try_free(*forms);
		*forms = NULL;
	}
}

static GEN p1363_func_F(GEN z, long precision) {
	pari_sp ltop = avma;

	if (gequal0(z)) {
		return gcopy(gen_1);
	}
	GEN sum = cgetc(precision);
	gel(sum, 1) = real_1(precision);
	gel(sum, 2) = real_0(precision);
	debug_log("initial sum = %Ps\n", sum);
	GEN last;

	GEN j = gcopy(gen_1);
	do {
		last = gcopy(sum);

		GEN j3 = mulsi(3, sqri(j));
		GEN quota = divis(subii(j3, j), 2);
		GEN quotb = divis(addii(j3, j), 2);

		GEN term = gadd(gpow(z, quota, precision), gpow(z, quotb, precision));

		if (mod2(j) == 0) {
			debug_log("%Ps (add) %Ps \n", sum, term);
			sum = gadd(sum, term);
		} else {
			debug_log("%Ps (sub) %Ps \n", sum, term);
			sum = gsub(sum, term);
		}

		addiiz(j, gen_1, j);
		if (gc_needed(ltop, 1)) gerepileall(ltop, 3, &j, &sum, &last);
	} while (!gequal(sum, last));

	debug_log("end sum = %Ps, last = %Ps\n", sum, last);

	return gerepilecopy(ltop, sum);
}

static GEN p1363_func_fzero(GEN D, p1363_form_t *form, long precision) {
	pari_sp ltop = avma;

	GEN upper = p1363_func_F(gneg(form->theta), precision);
	GEN lower = p1363_func_F(gsqr(form->theta), precision);
	GEN front = gpow(form->theta, mkfrac(gen_m1, stoi(24)), precision);

	debug_log("F(-theta) = %Ps\n", upper);
	debug_log("F(theta^2) = %Ps\n", lower);

	GEN divd = gdiv(upper, lower);
	debug_log("F(-theta) / F(theta^2) = %Ps\n", divd);

	GEN result = gmul(divd, front);

	// TODO: WHY????
	gel(result, 2) = gneg(gel(result, 2));

	return gerepilecopy(ltop, result);
}

static GEN p1363_func_fone(GEN D, p1363_form_t *form, long precision) {
	pari_sp ltop = avma;

	GEN upper = p1363_func_F(form->theta, precision);
	GEN lower = p1363_func_F(gsqr(form->theta), precision);
	GEN front = gpow(form->theta, mkfrac(gen_m1, stoi(24)), precision);

	debug_log("F(theta) = %Ps\n", upper);
	debug_log("F(theta^2) = %Ps\n", lower);

	GEN divd = gdiv(upper, lower);
	debug_log("F(theta) / F(theta^2) = %Ps\n", divd);

	GEN result = gmul(front, divd);

	// TODO: WHY????
	gel(result, 2) = gneg(gel(result, 2));

	return gerepilecopy(ltop, result);
}

static GEN p1363_func_ftwo(GEN D, p1363_form_t *form, long precision) {
	pari_sp ltop = avma;

	GEN upper = p1363_func_F(gpowgs(form->theta, 4), precision);
	GEN lower = p1363_func_F(gsqr(form->theta), precision);
	GEN front = gmul(gsqrt(gen_2, precision),
	                 gpow(form->theta, mkfrac(gen_1, stoi(12)), precision));

	debug_log("F(theta^4) = %Ps\n", upper);
	debug_log("F(theta^2) = %Ps\n", lower);

	GEN divd = gdiv(upper, lower);
	debug_log("F(theta^4) / F(theta^2) = %Ps\n", divd);

	GEN result = gmul(divd, front);

	// TODO: WHY????
	gel(result, 2) = gneg(gel(result, 2));

	return gerepilecopy(ltop, result);
}

static void p1363_m8(GEN D, p1363_form_t *form) { form->m8 = mod8(D); }

static void p1363_I(GEN D, p1363_form_t *form) {
	switch (form->m8) {
		case 1:
		case 2:
		case 6:
		case 7:
			form->I = 3;
			break;
		case 5:
			form->I = 6;
			break;
		case 3:
			if (dvdis(D, 3)) {
				form->I = 2;
			} else {
				form->I = 0;
			}
			break;
		default:
			pari_err_DOMAIN("p1636_I", "D", "==", stoi(8), D);
	}
}

static void p1363_J(GEN D, p1363_form_t *form) {
	pari_sp ltop = avma;
	GEN ac = mulii(form->A, form->C);

	if (mod2(ac) == 1) {
		form->J = 0;
	} else {
		if (mod2(form->C) == 0) {
			form->J = 1;
		} else if (mod2(form->A) == 0) {
			form->J = 2;
		} else {
			pari_err_BUG("hwat?");
		}
	}
	avma = ltop;
}

static void p1363_K(GEN D, p1363_form_t *form) {
	switch (form->m8) {
		case 1:
		case 2:
		case 6:
			form->K = 2;
			break;
		case 3:
		case 7:
			form->K = 1;
			break;
		case 5:
			form->K = 5;
			break;
		default:
			pari_err_DOMAIN("p1636_I", "D", "==", stoi(8), D);
	}
}

void p1363_L(GEN D, p1363_form_t *form) {
	pari_sp ltop = avma;
	GEN ac = mulii(form->A, form->C);
	long a2 = mod2(form->A);
	long c2 = mod2(form->C);

	if (mod2(ac) == 1 || (form->m8 == 5 && mod2(form->C) == 0)) {
		form->L = addii(subii(form->A, form->C), mulii(sqri(form->A), form->C));
	} else {
		if (a2 == 0) {
			if (form->m8 == 3) {
				form->L = addii(subii(form->A, form->C),
				                mulsi(5, mulii(form->A, sqri(form->C))));
			} else {
				form->L = subii(subii(form->A, form->C),
				                mulii(form->A, sqri(form->C)));
			}
		} else if (c2 == 0) {
			form->L = subii(addii(form->A, mulsi(2, form->C)),
			                mulii(form->A, sqri(form->C)));
		} else {
			pari_err_BUG("what?");
		}
	}
	form->L = gerepileupto(ltop, form->L);
}

static void p1363_M(GEN D, p1363_form_t *form) {
	pari_sp ltop = avma;
	GEN quot;
	if (mod2(form->A) == 0) {
		quot = divis(subis(sqri(form->C), 1), 8);
	} else {
		quot = divis(subis(sqri(form->A), 1), 8);
	}
	form->M = gerepileupto(ltop, powii(gen_m1, quot));
}

static void p1363_N(GEN D, p1363_form_t *form) {
	pari_sp ltop = avma;
	long ac2 = mod2(mulii(form->A, form->C));

	switch (form->m8) {
		case 5:
			form->N = gen_1;
			break;
		case 1:
		case 2:
		case 6:
			form->N = gcopy(form->M);
			break;
		case 3:
			if (ac2 == 0) {
				form->N = negi(form->M);
			} else {
				form->N = gen_1;
			}
			break;
		case 7:
			if (ac2 == 0) {
				form->N = gen_1;
			} else {
				form->N = gcopy(form->M);
			}
			break;
		default:
			pari_err_DOMAIN("p1363_N", "D", "=", stoi(8), D);
	}
	form->N = gerepileupto(ltop, form->N);
}

static void p1363_lambda(GEN D, p1363_form_t *form, long precision) {
	pari_sp ltop = avma;
	GEN pik = mulri(mppi(precision), stoi(form->K));
	GEN quot = divrs(pik, 24);
	form->lambda = gerepileupto(ltop, expIr(quot));
}

static void p1363_theta(GEN D, p1363_form_t *form, long precision) {
	pari_sp ltop = avma;

	GEN upper = gadd(gneg(gsqrt(D, precision)), gmul(form->B, gen_I()));
	GEN quot = gmul(gdiv(upper, form->A), mppi(precision));

	form->theta = gerepilecopy(ltop, gexp(quot, precision));
}

long p1363_bit_precision(GEN D, p1363_form_t **forms, size_t nforms) {
	pari_sp ltop = avma;
	long v0 = 64;
	GEN mul = divrr(mulrr(mppi(BIGDEFAULTPREC), gsqrt(D, BIGDEFAULTPREC)),
	                mplog2(BIGDEFAULTPREC));
	GEN sum = gen_0;
	for (size_t i = 0; i < nforms; ++i) {
		sum = gadd(sum, ginv(forms[i]->A));
	}
	long result = v0 + itos(gceil(gmul(mul, sum)));
	avma = ltop;
	return nbits2prec(result);
}

GEN p1363_invariant(GEN D, p1363_form_t *form, long precision) {
	debug_log("[A,B,C] = %Pi %Pi %Pi\n", form->A, form->B, form->C);
	pari_sp ltop = avma;

	p1363_m8(D, form);
	p1363_I(D, form);
	debug_log("I = %li\n", form->I);
	p1363_J(D, form);
	debug_log("J = %li\n", form->J);
	p1363_K(D, form);
	debug_log("K = %li\n", form->K);
	p1363_L(D, form);
	debug_log("L = %Pi\n", form->L);
	p1363_M(D, form);
	debug_log("M = %Pi\n", form->M);
	p1363_N(D, form);
	debug_log("N = %Pi\n", form->N);
	p1363_lambda(D, form, precision);
	debug_log("lambda = %Ps\n", form->lambda);
	p1363_theta(D, form, precision);
	debug_log("theta = %Ps\n", form->theta);

	GEN G = gcdii(D, stoi(3));
	debug_log("G = %Pi\n", G);

	GEN bl = mulii(form->B, form->L);

	GEN lmbl = gpow(form->lambda, negi(bl), precision);
	debug_log("lmbl = %Ps\n", lmbl);

	GEN mi6 = gneg(mkfrac(stoi(form->I), stoi(6)));
	GEN powmi6 = gpow(gen_2, mi6, precision);
	debug_log("powmi6 = %Ps\n", powmi6);

	GEN f = gen_0;
	switch (form->J) {
		case 0:
			f = p1363_func_fzero(D, form, precision);
			break;
		case 1:
			f = p1363_func_fone(D, form, precision);
			break;
		case 2:
			f = p1363_func_ftwo(D, form, precision);
			break;
		default:
			pari_err_DOMAIN("p1363_invariant", "J", ">", stoi(2),
			                stoi(form->J));
	}

	debug_log("f = %Ps\n", f);

	GEN fK = gpow(f, stoi(form->K), precision);
	debug_log("f^K = %Ps\n", fK);

	GEN in = gmul(lmbl, powmi6);
	debug_log("l^-BL * 2^-I/6 = %Ps\n", in);
	GEN inner = gmul(form->N, in);
	debug_log("N * l^-BL * 2^-I/6 = %Ps\n", inner);
	GEN result = gpow(gmul(inner, fK), G, precision);
	debug_log("result = %Ps\n", result);

	return gerepilecopy(ltop, result);
}

GEN p1363_poly(GEN D, p1363_form_t **forms, size_t nforms) {
	pari_sp ltop = avma;
	long v = fetch_var();
	name_var(v, "t");

	long precision = p1363_bit_precision(D, forms, nforms);

	GEN terms = gtovec0(gen_0, nforms);
	for (size_t i = 0; i < nforms; ++i) {
		GEN invariant = p1363_invariant(D, forms[i], precision);
		gel(terms, i + 1) = gsub(pol_x(v), invariant);
	}

	GEN approximate = gen_1;
	for (size_t i = 0; i < nforms; ++i) {
		debug_log("%Ps\n", gel(terms, i + 1));
		approximate = gmul(approximate, gel(terms, i + 1));
	}
	GEN vec = gtovec(approximate);
	long veclen = glength(vec);
	for (long i = 1; i <= veclen; ++i) {
		gel(vec, i) = ground(gel(vec, i));
	}

	GEN result = pol_0(v);
	for (long i = 1; i <= veclen; ++i) {
		result = gadd(result, gmul(gel(vec, i), pol_xn(veclen - i, v)));
	}
	return gerepilecopy(ltop, result);
}

GEN p1363_polclass(GEN D) {
	pari_sp ltop = avma;
	p1363_form_t **forms;
	size_t nforms = p1363_forms(D, &forms);
	GEN WD = p1363_poly(D, forms, nforms);
	p1363_free(&forms, nforms);
	return gerepileupto(ltop, WD);
}