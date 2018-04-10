/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "custom.h"
#include "io/output.h"
#include "obj/curve.h"
#include "obj/point.h"
#include "obj/subgroup.h"
#include "util/bits.h"

static size_t custom_add_primes(GEN r, GEN order, GEN **primes,
                                size_t nprimes) {
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

static void custom_quadr_init(custom_quadr_t *quadr, GEN order) {
	quadr->D = gen_0;
	quadr->p = gen_0;
	quadr->t = gen_0;

	quadr->order = order;
	quadr->r = gen_0;
	quadr->i = gen_0;
	quadr->Sp = NULL;
	quadr->nprimes = 0;
}

static void custom_quadr_next(custom_quadr_t *quadr) {
	// Ok I get some state in r, i, Sp and nprimes.
	// Here I want to check if I want to generate more primes into Sp
	// Then continue with i

	GEN logN = ground(glog(quadr->order, BIGDEFAULTPREC));
	GEN rlog2 = sqri(mulii(addis(quadr->r, 1), logN));

	// When do I want more primes? If i == imax, or nprimes == 0
	GEN imax = int2n(quadr->nprimes);
	if (equalii(quadr->i, imax) || quadr->nprimes == 0) {
		quadr->nprimes = custom_add_primes(quadr->r, quadr->order, &(quadr->Sp),
		                                   quadr->nprimes);
	}

	while (true) {
		imax = int2n(quadr->nprimes);

		while (cmpii(quadr->i, imax) < 0) {
			// debug_log("i %Pi", quadr->i);
			pari_sp btop = avma;
			GEN pprod = gen_1;
			bits_t *ibits = bits_from_i_len(quadr->i, quadr->nprimes);
			for (size_t j = 0; j < quadr->nprimes; ++j) {
				if (GET_BIT(ibits->bits, j) == 1) {
					// debug_log("multiplying %Pi", quadr->Sp[j]);
					pprod = mulii(pprod, quadr->Sp[j]);
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
				if (!cornacchia2(absp, quadr->order, &x, &y)) {
					avma = btop;
					quadr->i = addis(quadr->i, 1);
					// debug_log("Cornacchia fail");
					continue;
				}
				GEN pp1 = addii(addis(quadr->order, 1), x);
				GEN pp2 = subii(addis(quadr->order, 1), x);
				if (isprime(pp1)) {
					quadr->p = pp1;
					quadr->D = pprod;
					quadr->t = x;
					quadr->i = addis(quadr->i, 1);
					debug_log("good D %Pi", pprod);
					return;
				}
				if (isprime(pp2)) {
					quadr->p = pp2;
					quadr->D = pprod;
					quadr->t = x;
					quadr->i = addis(quadr->i, 1);
					debug_log("good D %Pi", pprod);
					return;
				}
			}
			avma = btop;
			quadr->i = addis(quadr->i, 1);
		}

		quadr->r = addis(quadr->r, 1);
		quadr->nprimes = custom_add_primes(quadr->r, quadr->order, &(quadr->Sp),
		                                   quadr->nprimes);
		rlog2 = sqri(mulii(addis(quadr->r, 1), logN));
	}
}

static void custom_quadr_free(custom_quadr_t *quadr) { try_free(quadr->Sp); }

curve_t *custom_curve() {
	GEN order = strtoi(cfg->cm_order);
	if (!isprime(order)) {
		fprintf(err, "Currently, order must be prime for CM to work.\n");
		return NULL;
	}

	GEN a = NULL;
	GEN e = NULL;
	GEN g = NULL;

	custom_quadr_t quadr;
	custom_quadr_init(&quadr, order);
	while (true) {
		custom_quadr_next(&quadr);

		debug_log("order = %Pi", order);
		debug_log("p = %Pi, t = %Pi, D = %Pi, ", quadr.p, quadr.t, quadr.D);
		GEN H = polclass(quadr.D, 0, 0);

		debug_log("H = %Ps", H);

		GEN r = FpX_roots(H, quadr.p);
		debug_log("roots = %Ps", r);
		if (gequal(r, gtovec(gen_0))) {
			continue;
		}

		bool has_curve = false;

		long rlen = glength(r);
		for (long i = 1; i <= rlen; ++i) {
			GEN root = gel(r, i);
			a = Fp_div(
			    Fp_mul(stoi(27), root, quadr.p),
			    Fp_mul(stoi(4), Fp_sub(stoi(1728), root, quadr.p), quadr.p),
			    quadr.p);
			e = ellinit(mkvec2(a, negi(a)), quadr.p, 0);
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

	custom_quadr_free(&quadr);

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