/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "custom.h"
#include "io/input.h"
#include "io/output.h"
#include "obj/curve.h"
#include "obj/point.h"
#include "obj/subgroup.h"
#include "util/bits.h"

static size_t custom_add_primes(GEN r, GEN order, GEN **primes,
                                size_t nprimes) {
	size_t nalloc = nprimes;
	if (nprimes == 0) {
		nalloc = 10;
		*primes = try_calloc(sizeof(GEN) * nalloc);
		debug_log("calloc %lu", sizeof(GEN) * nalloc);
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
			(*primes)[nprimes++] = pstar;
			if (nprimes == nalloc) {
				nalloc *= 2;
				*primes = try_realloc(*primes, sizeof(GEN) * nalloc);
			}
		}
	}

	return nprimes;
}

static custom_quadr_t custom_prime_random(GEN order) {
	pari_sp ltop = avma;
	custom_quadr_t result = {0};

	GEN r = gen_0;
	GEN *Sp;
	size_t nprimes = custom_add_primes(r, order, &Sp, 0);

	GEN logN = ground(glog(order, BIGDEFAULTPREC));
	GEN rlog2 = sqri(mulii(r, logN));

	GEN i = gen_0;

	while (true) {
		GEN imax = int2n(nprimes);

		while (cmpii(i, imax) < 0) {
			// debug_log("i %Pi", i);
			pari_sp btop = avma;
			GEN pprod = gen_1;
			bits_t *ibits = bits_from_i_len(i, nprimes);
			for (size_t j = 0; j < nprimes; ++j) {
				if (GET_BIT(ibits->bits, j) == 1) {
					// debug_log("multiplying %Pi", Sp[j]);
					pprod = mulii(pprod, Sp[j]);
				}
			}
			bits_free(&ibits);
			if (cmpii(pprod, rlog2) < 0 && equalii(modis(pprod, 8), stoi(5))) {
				// debug_log("candidate D = %Pi", pprod);
				GEN x;
				GEN y;
				cornacchia2(negi(pprod), order, &x, &y);
				GEN pp1 = addii(addis(order, 1), x);
				GEN pp2 = subii(addis(order, 1), x);
				if (isprime(pp1)) {
					result.p = pp1;
					result.D = pprod;
					result.t = x;
					result.v = gen_0;
					gerepileall(ltop, 4, &result.p, &result.t, &result.v,
					            &result.D);
					try_free(Sp);
					return result;
				}
				if (isprime(pp2)) {
					result.p = pp2;
					result.D = pprod;
					result.t = x;
					result.v = gen_0;
					gerepileall(ltop, 4, &result.p, &result.t, &result.v,
					            &result.D);
					try_free(Sp);
					return result;
				}
			}
			avma = btop;
			i = addis(i, 1);
		}

		r = addis(r, 1);
		nprimes = custom_add_primes(r, order, &Sp, nprimes);
	}
}

curve_t *custom_curve() {
	GEN order = strtoi(cfg->cm_order);

	custom_quadr_t quadr = custom_prime_random(order);
	debug_log("order = %Pi", order);
	debug_log("p = %Pi, t = %Pi, v = %Pi, D = %Pi, ", quadr.p, quadr.t, quadr.v,
	          quadr.D);
	GEN H = polclass(quadr.D, 0, 0);

	debug_log("H = %Ps", H);

	GEN r = FpX_roots(H, quadr.p);
	debug_log("roots = %Ps", r);

	GEN a = NULL;
	GEN e = NULL;
	GEN g = NULL;

	long rlen = glength(r);
	for (long i = 1; i <= rlen; ++i) {
		GEN root = gel(r, i);
		a = Fp_div(Fp_mul(stoi(27), root, quadr.p),
		           Fp_mul(stoi(4), Fp_sub(stoi(1728), root, quadr.p), quadr.p),
		           quadr.p);
		e = ellinit(mkvec2(a, negi(a)), quadr.p, 0);
		g = genrand(e);
		GEN gmul = ellmul(e, g, order);
		if (gequal(gmul, gtovec(gen_0))) {
			debug_log("YES %Ps", e);
			break;
		}
	}

	curve_t *result = curve_new();
	result->field = quadr.p;
	result->a = a;
	result->b = negi(a);
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