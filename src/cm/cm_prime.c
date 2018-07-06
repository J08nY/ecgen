/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "cm_prime.h"
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
				GEN x;
				GEN y;
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

curve_t *cm_prime_curve(GEN order) {
	GEN a = NULL;
	GEN b = NULL;
	GEN e = NULL;
	GEN g = NULL;

	cm_prime_qdisc_t qdisc;
	qdisc_init(&qdisc, order);
	while (true) {
		qdisc_next(&qdisc);

		debug_log("order = %Pi", order);
		debug_log("p = %Pi, t = %Pi, D = %Pi, ", qdisc.p, qdisc.t, qdisc.D);
		GEN H = polclass(qdisc.D, 0, 0);

		debug_log("H = %Ps", H);

		GEN r = FpX_roots(H, qdisc.p);
		debug_log("roots = %Ps", r);
		if (gequal(r, gtovec(gen_0))) {
			continue;
		}

		bool has_curve = false;

		long rlen = glength(r);
		for (long i = 1; i <= rlen; ++i) {
			GEN root = gel(r, i);
			a = mkintmod(
			    Fp_div(
			        Fp_mul(stoi(27), root, qdisc.p),
			        Fp_mul(stoi(4), Fp_sub(stoi(1728), root, qdisc.p), qdisc.p),
			        qdisc.p),
			    qdisc.p);
			b = gneg(a);
			e = ellinit(mkvec2(a, b), qdisc.p, 0);
			pari_CATCH(e_TYPE) { continue; }
			pari_TRY { checkell(e); };
			pari_ENDCATCH{};

			g = genrand(e);
			GEN gmul = ellmul(e, g, order);
			if (ell_is_inf(gmul)) {
				debug_log("YES %Ps", e);
				has_curve = true;
				break;
			}
		}

		if (has_curve) break;
	}

	qdisc_free(&qdisc);

	curve_t *result = curve_new();
	result->field = qdisc.p;
	result->a = a;
	result->b = b;
	result->curve = e;
	result->order = order;
	result->generators = subgroups_new(1);
	result->generators[0] = subgroup_new();
	result->generators[0]->generator = point_new();
	result->generators[0]->generator->point = g;
	result->generators[0]->generator->order = order;
	result->generators[0]->generator->cofactor = stoi(1);
	result->generators[0]->npoints = 0;
	result->ngens = 1;

	return result;
}