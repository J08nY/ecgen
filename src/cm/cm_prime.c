/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "cm_prime.h"
#include "cm_any.h"
#include "exhaustive/arg.h"
#include "io/output.h"
#include "obj/curve.h"
#include "obj/point.h"
#include "obj/subgroup.h"
#include "util/bits.h"

static size_t add_primes(GEN r, GEN order, GEN **primes, size_t nprimes) {
	debug_log("add_primes r = %Pi, nprimes = %lu", r, nprimes);
	size_t nalloc = nprimes;
	if (nprimes == 0) {
		nalloc = 10;
		*primes = try_calloc(sizeof(GEN) * nalloc);
	}

	GEN logN = ground(glog(order, BIGDEFAULTPREC));

	GEN rlog = mulii(r, logN);
	GEN rplog = mulii(addis(r, 1), logN);

	forprime_t iter;
	forprime_init(&iter, rlog, rplog);
	GEN prime;
	while ((prime = forprime_next(&iter))) {
		long k = kronecker(order, prime);
		if (k == 1) {
			GEN pstar = prime;
			GEN ppow = divis(subis(prime, 1), 2);
			if (mod2(ppow) == 1) {
				pstar = negi(prime);
			} else {
				pstar = gcopy(pstar);
			}
			if (nprimes == nalloc) {
				nalloc *= 2;
				*primes = try_realloc(*primes, sizeof(GEN) * nalloc);
			}
			(*primes)[nprimes++] = pstar;
		}
	}

	*primes = try_realloc(*primes, sizeof(GEN) * nprimes);

	return nprimes;
}

static void qdisc_init(cm_prime_qdisc_t *qdisc, GEN order) {
	qdisc->D = gen_0;
	qdisc->p = gen_0;
	qdisc->t = gen_0;

	qdisc->order = order;
	qdisc->r = gen_0;
	qdisc->i = gen_0;
	qdisc->Sp = NULL;
	qdisc->nprimes = 0;
}

static void qdisc_next(cm_prime_qdisc_t *qdisc) {
	// Ok I get some state in r, i, Sp and nprimes.
	// Here I want to check if I want to generate more primes into Sp
	// Then continue with i

	GEN logN = ground(glog(qdisc->order, BIGDEFAULTPREC));
	GEN rlog2 = sqri(mulii(addis(qdisc->r, 1), logN));

	// When do I want more primes? If i == imax, or nprimes == 0
	GEN imax = int2n(qdisc->nprimes);
	if (equalii(qdisc->i, imax) || qdisc->nprimes == 0) {
		qdisc->nprimes =
		    add_primes(qdisc->r, qdisc->order, &(qdisc->Sp), qdisc->nprimes);
		imax = int2n(qdisc->nprimes);
	}

	pari_sp btop = avma;
	while (true) {
		while (cmpii(qdisc->i, imax) < 0) {
			// debug_log("i %Pi", qdisc->i);
			GEN pprod = gen_1;
			bits_t *ibits = bits_from_i_len(qdisc->i, qdisc->nprimes);
			for (size_t j = 0; j < qdisc->nprimes; ++j) {
				if (GET_BIT(ibits->bits, j) == 1) {
					// debug_log("multiplying %Pi", qdisc->Sp[j]);
					pprod = mulii(pprod, qdisc->Sp[j]);
				}
			}
			bits_free(&ibits);

			GEN absp = absi(pprod);
			long m4 = mod4(absp);
			if (cmpii(absp, rlog2) < 0 && equalii(modis(pprod, 8), stoi(5)) &&
			    m4 != 1 && m4 != 2) {
				debug_log("candidate D = %Pi", pprod);
				GEN x = NULL;
				GEN y = NULL;
				if (!cornacchia2(absp, qdisc->order, &x, &y)) {
					qdisc->i = gerepileupto(btop, addis(qdisc->i, 1));
					// debug_log("Cornacchia fail");
					continue;
				}
				GEN pp1 = addii(addis(qdisc->order, 1), x);
				GEN pp2 = subii(addis(qdisc->order, 1), x);
				if (isprime(pp1)) {
					qdisc->p = pp1;
					qdisc->D = pprod;
					qdisc->t = x;
					qdisc->i = addis(qdisc->i, 1);
					debug_log("good D %Pi", pprod);
					return;
				}
				if (isprime(pp2)) {
					qdisc->p = pp2;
					qdisc->D = pprod;
					qdisc->t = x;
					qdisc->i = addis(qdisc->i, 1);
					debug_log("good D %Pi", pprod);
					return;
				}
			}
			qdisc->i = gerepileupto(btop, addis(qdisc->i, 1));
		}

		qdisc->r = addis(qdisc->r, 1);
		qdisc->nprimes =
		    add_primes(qdisc->r, qdisc->order, &(qdisc->Sp), qdisc->nprimes);
		rlog2 = sqri(mulii(addis(qdisc->r, 1), logN));
		imax = int2n(qdisc->nprimes);

		btop = avma;
	}
}

static void qdisc_free(cm_prime_qdisc_t *qdisc) { try_free(qdisc->Sp); }

GENERATOR(cm_gen_curve_prime) {
	HAS_ARG(args);
	const char *order_s = (const char *)args->args;
	GEN order = strtoi(order_s);
	GEN e = NULL;

	cm_prime_qdisc_t qdisc = {0};
	qdisc_init(&qdisc, order);
	do {
		qdisc_next(&qdisc);
		e = cm_construct_curve(order, qdisc.D, qdisc.p, true);
	} while (e == NULL);
	qdisc_free(&qdisc);

	curve->field = qdisc.p;
	curve->a = ell_get_a4(e);
	curve->b = ell_get_a6(e);
	curve->curve = e;
	return 1;
}