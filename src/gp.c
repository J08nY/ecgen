#include "gp.h"

void init_gp(void) /* void */
{
	pari_sp ltop = avma;
	avma = ltop;
	return;
}

/*
* ecgen, tool for generating Elliptic curve domain parameters
* Copyright (C) 2017  J08nY
*/

/*
* ecgen, tool for generating Elliptic curve domain parameters
* Copyright (C) 2017 J08nY
*/

GEN random_primer(GEN range) /* int */
{
	pari_sp ltop = avma;
	GEN p = gen_0; /* int */
	if (!is_matvec_t(typ(range))) pari_err_TYPE("random_primer", range);
	{
		pari_sp btop = avma;
		do {
			p = randomprime(range);
			if (gc_needed(btop, 1)) p = gerepilecopy(btop, p);
		} while (!isprime(p));
	}
	p = gerepilecopy(ltop, p);
	return p;
}

/**
* Calculates a random prime of bit size bits.
*
* @param bits bit size of the requested prime
* @return random prime between 2^(bits - 1) and 2^bits
*/
GEN random_prime(long bits) /* int */
{
	pari_sp ltop = avma;
	GEN p1 = gen_0; /* vec */
	GEN p2 = gen_0; /* int */
	p1 = cgetg(3, t_VEC);
	gel(p1, 1) = powis(gen_2, bits - 1);
	gel(p1, 2) = powis(gen_2, bits);
	p2 = random_primer(p1);
	p2 = gerepilecopy(ltop, p2);
	return p2;
}

GEN random_intr(GEN range) {
	pari_sp ltop = avma;
	GEN p1 = gen_0;
	if (!is_matvec_t(typ(range))) pari_err_TYPE("random_intr", range);
	p1 = genrand(range);
	p1 = gerepilecopy(ltop, p1);
	return p1;
}

/**
* Generates a random integer with bit size bits.
*
* @param bits bit size of the requested integer
* @return random int between 2^(bits - 1) and 2^bits
*/
GEN random_int(long bits) {
	pari_sp ltop = avma;
	GEN p1 = gen_0; /* vec */
	GEN p2 = gen_0;
	p1 = cgetg(3, t_VEC);
	gel(p1, 1) = powis(gen_2, bits - 1);
	gel(p1, 2) = powis(gen_2, bits);
	p2 = random_intr(p1);
	p2 = gerepilecopy(ltop, p2);
	return p2;
}

/**
* Converts a list to a vector.
*
* @param l list to convert
* @return a vector of the lists values
*/
GEN list_to_vec(GEN l) /* vec */
{
	pari_sp ltop = avma;
	GEN v = gen_0;  /* vec */
	GEN n = gen_0;  /* int */
	GEN p1 = gen_0; /* vec */
	n = stoi(glength(l));
	{
		long l2;
		p1 = cgetg(itos(n) + 1, t_VEC);
		for (l2 = 1; cmpsi(l2, n) <= 0; ++l2) gel(p1, l2) = gen_0;
	}
	v = p1;
	{
		pari_sp btop = avma;
		GEN i = gen_0;
		for (i = gen_1; gcmp(i, n) <= 0; i = gaddgs(i, 1)) {
			gel(v, gtos(i)) = gcopy(gel(list_data(l), gtos(i)));
			if (gc_needed(btop, 1)) gerepileall(btop, 2, &v, &i);
		}
	}
	v = gerepilecopy(ltop, v);
	return v;
}

/*
* ecgen, tool for generating Elliptic curve domain parameters
* Copyright (C) 2017 J08nY
*/

/**
* Computes primes upto some upper bound.
*
* @param bound an upper bound on primes
* @return a vector of primes up to bound^2
*/
GEN prime_upto(GEN bound) /* vec */
{
	pari_sp ltop = avma;
	GEN p = gen_0;                     /* list */
	GEN product = gen_0, last = gen_0; /* int */
	GEN result = gen_0;                /* vec */
	if (typ(bound) != t_INT) pari_err_TYPE("prime_upto", bound);
	p = mklist();
	bound = sqri(bound);
	listput0(p, gen_2, 0);
	product = gen_2;
	last = gen_2;
	{
		pari_sp btop = avma;
		while (cmpii(product, bound) < 0) {
			last = nextprime(addis(last, 1));
			listput0(p, last, 0);
			product = mulii(product, last);
			if (gc_needed(btop, 1)) gerepileall(btop, 3, &p, &product, &last);
		}
	}
	result = list_to_vec(p);
	listkill(p);
	result = gerepilecopy(ltop, result);
	return result;
}

/**
*
*/
GEN invalid(GEN coeffs, GEN field, GEN primes, GEN bits, long prec) /* vec */
{
	pari_sp ltop = avma;
	GEN bs = gen_0, cs = gen_0, eq = gen_0;         /* vec */
	GEN e = gen_0;                                  /* ell */
	GEN b = gen_0, n = gen_0, c = gen_0, o = gen_0; /* int */
	GEN p1 = gen_0;                                 /* vec */
	if (!is_matvec_t(typ(coeffs))) pari_err_TYPE("invalid", coeffs);
	if (typ(field) != t_POL) pari_err_TYPE("invalid", field);
	if (!is_matvec_t(typ(primes))) pari_err_TYPE("invalid", primes);
	if (typ(bits) != t_INT) pari_err_TYPE("invalid", bits);
	n = stoi(lg(primes) - 1);
	{
		long l2;
		p1 = cgetg(itos(n) + 1, t_VEC);
		for (l2 = 1; cmpsi(l2, n) <= 0; ++l2) gel(p1, l2) = gen_0;
	}
	bs = p1;
	eq = gcopy(coeffs);
	c = gen_0;
	{
		pari_sp btop = avma;
		while (cmpii(c, n) < 0) {
			b = random_int(itos(bits));
			gel(eq, 4) = icopy(b);
			/* Times field? */

			pari_CATCH(CATCH_ALL) {
				GEN E = pari_err_last(); /* error */
				continue;
			}
			pari_TRY { e = ellinit(eq, field, prec); }
			pari_ENDCATCH o = ellsea(e, 0);
			{
				pari_sp btop = avma;
				GEN i = gen_0;
				for (i = gen_1; gcmp(i, n) <= 0; i = gaddgs(i, 1)) {
					if (gequal0(gmod(o, gel(primes, gtos(i)))) &&
					    gequal0(gel(bs, gtos(i)))) {
						gel(bs, gtos(i)) = icopy(b);
						gel(cs, gtos(i)) = gcopy(e);
						c = addis(c, 1);
					}
					if (gc_needed(btop, 1))
						gerepileall(btop, 4, &bs, &cs, &c, &i);
				}
			}
			if (gc_needed(btop, 1))
				gerepileall(btop, 7, &bs, &cs, &eq, &e, &b, &c, &o);
		}
	}
	cs = gerepilecopy(ltop, cs);
	return cs;
}
