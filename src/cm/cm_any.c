/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "cm_any.h"
#include "exhaustive/arg.h"
#include "io/output.h"
#include "obj/curve.h"
#include "util/memory.h"

/**
 * @brief Slightly adapted algorithm from section 4.2 of
 *        Constructing elliptic curves of prescribed order,
 *        Reiner Broker
 * @param order
 */
static void good_qdisc_minimal(cm_any_qdisc_t *qdisc, GEN order) {
	pari_sp ltop = avma;
	GEN d = stoi(2);
	size_t j = 0;
	while (true) {
		++j;
		if (!issquarefree(d)) {
			d = addis(d, 1);
			continue;
		}
		if (j % 100 == 0) {
			debug_log("D: %Ps", d);
		}
		GEN D = quaddisc(negi(d));
		GEN K = Buchall(quadpoly(D), 0, DEFAULTPREC);
		GEN alphas = bnfisintnorm(K, order);
		long len = glength(alphas);
		if (len != 0) {
			debug_log("Got some elems of norm N: %Ps", alphas);
			for (long i = 1; i <= len; ++i) {
				GEN alpha = gel(alphas, i);
				GEN trace = nftrace(K, alpha);
				GEN p = subii(addis(order, 1), trace);
				if (isprime(p)) {
					debug_log(
					    "Got an elem of prime trace: %Pi, d = %Pi, D = %Pi", p,
					    d, D);
					qdisc->p = p;
					qdisc->d = D;
					gerepileall(ltop, 2, &qdisc->p, &qdisc->d);
					return;
				}
			}
		}
		d = gerepileupto(ltop, addis(d, 1));
	}
}

/**
 * @brief Find a fundamental quadratic discriminant < d_range, start looking
 *        at the sides of the inverted Hasse interval around order, upto p_range
 *
 * 	    order + 1 - 2*sqrt(order)        order + 1 + 2*sqrt(order)
 *      	   |                                 |
 *     		   |-----|----------|----------|-----|
 *  		   >_____>          |          <_____<
 * 		          |           order           |
 *		        p_range                     p_range
 * Inspired by a method from:
 *     Constructing elliptic curves of prescribed order,
 *     Reiner Broker
 *
 * @param order
 * @param p_range
 * @param d_range
 * @return
 *//*

static cm_any_qdisc_t *good_qdisc_brute_range(GEN order, GEN p_range, GEN d_range) {
	pari_sp ltop = avma;
	GEN tsqrt_ord = mulis(sqrti(order), 2);
	GEN left_p = subii(addis(order, 1), tsqrt_ord);
	GEN right_p = addii(addis(order, 1), tsqrt_ord);

	GEN left_max_p = addii(left_p, p_range);
	GEN right_min_p = subii(right_p, p_range);

	GEN min_d = stoi(0);
	bool left = true;
	while (true) {
		GEN p;
		if (left) {
			left_p = nextprime(addis(left_p, 1));
			p = left_p;
		} else {
			right_p = precprime(subis(right_p, 1));
			p = right_p;
		}
		left = !left;
		GEN t = subii(addis(p, 1), order);
		GEN D = subii(sqri(t), mulis(p, 4));
		GEN d = coredisc(D);
		if (gequal0(min_d) || cmpii(d, min_d) > 0) {
			debug_log("New min D = %Pi", d);
			min_d = d;
		}
		if (cmpii(absi(d), d_range) < 0) {
			debug_log("Good min D = %Pi", d);
		} else if (cmpii(left_p, left_max_p) > 0 || cmpii(right_p, right_min_p) < 0) {
			debug_log("Over p_range, D = %Pi", d);
		} else {
			continue;
		}
		cm_any_qdisc_t *result = try_calloc(sizeof(cm_any_qdisc_t));
		result->p = p;
		result->d = d;
		gerepileall(ltop, 2, &result->p, &result->d);
		return result;
	};
}

*/
/**
 * @brief Find a fundamental quadratic discriminant < order^beta, start looking
 * 		  at the sides of the inverted Hasse interval around order, upto
 * order^alpha width.
 * @param order
 * @param alpha
 * @param beta
 * @return
 *//*

static cm_any_qdisc_t *good_qdisc_brute(GEN order, GEN alpha, GEN beta) {
	GEN ord_a = ground(gpow(order, alpha, DEFAULTPREC));
	GEN ord_b = ground(gpow(order, beta, DEFAULTPREC));
	return good_qdisc_brute_range(order, ord_a, ord_b);
}
*/

GEN cm_construct_curve(GEN order, GEN d, GEN p, bool ord_prime) {
	debug_log("Constructing a curve with N = %Pi, d = %Pi, p = %Pi", order, d,
	          p);
	pari_sp ltop = avma;
	GEN H = polclass(d, 0, 0);
	debug_log("H = %Ps", H);

	GEN r = FpX_roots(H, p);
	debug_log("roots = %Ps", r);
	if (gequal(r, gtovec(gen_0))) {
		return NULL;
	}

	long rlen = glength(r);
	for (long i = 1; i <= rlen; ++i) {
		GEN root = gel(r, i);
		debug_log("trying root = %Pi", root);

		GEN e = ellinit(ellfromj(mkintmod(root, p)), p, 0);
		pari_CATCH(e_TYPE) { continue; }
		pari_TRY { checkell(e); };
		pari_ENDCATCH{};

		if (ord_prime) {
			// Easy part, the requested order is prime so
			// [order]G = 0 iff the curve has exactly order points, for any G on
			// it. otherwise it is the twist
			GEN g = genrand(e);
			if (ell_is_inf(ellmul(e, g, order))) {
				debug_log("Got curve.");
				return gerepilecopy(ltop, e);
			} else {
				debug_log("Got curve twist.");
				return gerepilecopy(ltop, ellinit(elltwist(e, NULL), p, 0));
			}
		} else {
			// Hard part, requested order is composite, so it might share a
			// factor with the order of the twist, which means [order]G = 0
			// might be true for a point on the twist as well as a point o the
			// right curve.
			//
			// We calculate what the twist order is, then compute gcd of the
			// orders which leads to the product of the factors that the orders
			// do not share. By multiplying a random point by this product on
			// some curve, we can determine that that curve has that number of
			// points.
			GEN twist_order = subii(addis(p, 1), subii(order, addis(p, 1)));
			GEN twist = ellinit(elltwist(e, NULL), p, 0);
			GEN gcd = gcdii(order, twist_order);
			GEN orig_mul = divii(order, gcd);
			GEN twist_mul = divii(twist_order, gcd);
			while (true) {
				GEN orig_point = genrand(e);
				if (ell_is_inf(ellmul(e, orig_point, orig_mul))) {
					debug_log("Got curve.");
					return gerepilecopy(ltop, e);
				}
				if (ell_is_inf(ellmul(e, orig_point, twist_mul))) {
					debug_log("Got curve twist.");
					return gerepilecopy(ltop, twist);
				}
				GEN twist_point = genrand(twist);
				if (ell_is_inf(ellmul(e, twist_point, twist_mul))) {
					debug_log("Got curve.");
					return gerepilecopy(ltop, e);
				}
				if (ell_is_inf(ellmul(e, twist_point, orig_mul))) {
					debug_log("Got curve twist.");
					return gerepilecopy(ltop, twist);
				}
			}
		}
	}
	return NULL;
}

GENERATOR(cm_gen_curve_any) {
	HAS_ARG(args);
	pari_sp ltop = avma;
	const char *order_s = (const char *)args->args;
	GEN order = strtoi(order_s);
	cm_any_qdisc_t min_disc = {0};
	good_qdisc_minimal(&min_disc, order);
	debug_log("Got min D = %Pi", min_disc.d);
	GEN e = cm_construct_curve(order, min_disc.d, min_disc.p, false);
	if (e == NULL) {
		fprintf(err, "Could not construct curve.");
		avma = ltop;
		return -3;
	}
	curve->field = min_disc.p;
	curve->a = ell_get_a4(e);
	curve->b = ell_get_a6(e);
	curve->curve = e;
	return 1;
}

GENERATOR(cm_gen_order) {
	HAS_ARG(args);
	const char *order_s = (const char *)args->args;
	curve->order = strtoi(order_s);
	return 1;
}