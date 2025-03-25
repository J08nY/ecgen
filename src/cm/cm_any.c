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
	GEN d;
	if (qdisc->d) {
		d = negi(subis(qdisc->d, 1));
	} else {
		d = stoi(2);
	}
	size_t j = 1;
	while (true) {
		++j;
		if (j % 100 == 0) {
			debug_log("d: %Ps", d);
		}
		if (!issquarefree(d)) {
			d = addis(d, 1);
			continue;
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

/*
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
/*
 *
 * @brief Find a fundamental quadratic discriminant < order^beta, start looking
 * 		  at the sides of the inverted Hasse interval around order, upto
 * order^alpha width.
 * @param order
 * @param alpha
 * @param beta
 * @return
 *
static cm_any_qdisc_t *good_qdisc_brute(GEN order, GEN alpha, GEN beta) {
    GEN ord_a = ground(gpow(order, alpha, DEFAULTPREC));
    GEN ord_b = ground(gpow(order, beta, DEFAULTPREC));
    return good_qdisc_brute_range(order, ord_a, ord_b);
}
*/

void cm_update_roots(GEN d, GEN p, cm_any_roots_t *roots) {
	pari_sp ltop = avma;
	GEN H = polclass(d, 0, 0);
	GEN raw = FpX_roots(H, p);
	if (roots->roots && isclone(roots->roots)) {
		gunclone(roots->roots);
	}
	roots->roots = gclone(raw);
	roots->total = glength(raw);
	roots->used = 0;
	avma = ltop;
}

cm_any_roots_t *cm_make_roots(GEN d, GEN p) {
	debug_log("Making roots, d = %Pi, p = %Pi", d, p);
	cm_any_roots_t *roots = try_calloc(sizeof(cm_any_roots_t));
	cm_update_roots(d, p, roots);
	return roots;
}

void cm_free_roots(cm_any_roots_t *roots) {
	if (roots) {
		if (roots->roots && isclone(roots->roots)) {
			gunclone(roots->roots);
		}
		try_free(roots);
	}
}

GEN cm_construct_curve(GEN order, GEN d, GEN p, cm_any_roots_t *roots,
                       bool ord_prime) {
	debug_log("Constructing a curve with N = %Pi, d = %Pi, p = %Pi", order, d,
	          p);
	pari_sp ltop = avma;

	debug_log("roots(%li/%li) = %Ps", roots->used, roots->total, roots->roots);
	if (roots->total == 0 || roots->used == roots->total ||
	    gequal(roots->roots, gtovec(gen_0))) {
		avma = ltop;
		return NULL;
	}

	for (long i = roots->used; i < roots->total; ++i) {
		roots->used = i + 1;
		GEN root = gel(roots->roots, i + 1);
		debug_log("trying root[%i] = %Pi", i + 1, root);

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
	avma = ltop;
	return NULL;
}

GEN cm_construct_curve_subgroup(GEN r, GEN d, GEN p, cm_any_roots_t *roots) {
	debug_log("Constructing a curve with r = %Pi, d = %Pi, p = %Pi", r, d, p);
	pari_sp ltop = avma;

	debug_log("roots(%li/%li) = %Ps", roots->used, roots->total, roots->roots);
	if (roots->total == 0 || roots->used == roots->total ||
	    gequal(roots->roots, gtovec(gen_0))) {
		avma = ltop;
		return NULL;
	}

	pari_sp btop = avma;
	for (long i = roots->used; i < roots->total; ++i) {
		roots->used = i + 1;
		GEN root = gel(roots->roots, i + 1);
		debug_log("trying root[%i] = %Pi", i + 1, root);

		GEN e = ellinit(ellfromj(mkintmod(root, p)), p, 0);
		pari_CATCH(e_TYPE) {
			avma = btop;
			continue;
		}
		pari_TRY { checkell(e); };
		pari_ENDCATCH{};

		GEN ord = ellff_get_card(e);
		if (dvdii(ord, r)) {
			debug_log("Got curve.");
			return gerepilecopy(ltop, e);
		}
		avma = btop;
	}
	avma = ltop;
	return NULL;
}

static cm_any_qdisc_t *min_d = NULL;
static cm_any_roots_t *min_roots = NULL;
static curve_t *min_curve = NULL;

GENERATOR(cm_gen_curve_any) {
	HAS_ARG(args);
	pari_sp ltop = avma;
	const char *order_s = (const char *)args->args;
	GEN order = strtoi(order_s);
	GEN e;
	if (min_d && min_roots && min_curve == curve &&
	    min_roots->used < min_roots->total) {
		debug_log("Reusing roots.");
		// We can just use the roots we have stored and take some out.
		e = cm_construct_curve(order, min_d->d, min_d->p, min_roots, false);
	} else if (min_d && min_curve == curve) {
		debug_log("Reusing min D = %Pi", min_d->d);
		// We just have the discriminant but no roots (or they are used up), we
		// need to continue
		if (min_d->d && isclone(min_d->d)) {
			gunclone(min_d->d);
		}
		if (min_d->p && isclone(min_d->p)) {
			gunclone(min_d->p);
		}
		good_qdisc_minimal(min_d, order);
		min_d->d = gclone(min_d->d);
		min_d->p = gclone(min_d->p);
		debug_log("Got min D = %Pi", min_d->d);
		if (min_roots) {
			cm_update_roots(min_d->d, min_d->p, min_roots);
		} else {
			min_roots = cm_make_roots(min_d->d, min_d->p);
		}
		e = cm_construct_curve(order, min_d->d, min_d->p, min_roots, false);
	} else {
		// We have nothing. Start fresh.
		debug_log("Fresh start.");
		if (!min_d) {
			min_d = try_calloc(sizeof(cm_any_qdisc_t));
		}
		if (min_d->d && isclone(min_d->d)) {
			gunclone(min_d->d);
		}
		if (min_d->p && isclone(min_d->p)) {
			gunclone(min_d->p);
		}
		good_qdisc_minimal(min_d, order);
		min_d->d = gclone(min_d->d);
		min_d->p = gclone(min_d->p);
		debug_log("Got min D = %Pi", min_d->d);
		min_roots = cm_make_roots(min_d->d, min_d->p);
		min_curve = curve;
		e = cm_construct_curve(order, min_d->d, min_d->p, min_roots, false);
	}

	if (e == NULL) {
		fprintf(err, "Could not construct curve.");
		avma = ltop;
		return -3;
	}
	curve->field = gcopy(min_d->p);
	curve->a = ell_get_a4(e);
	curve->b = ell_get_a6(e);
	curve->curve = e;
	return 1;
}

GENERATOR(cm_gen_curve_unique) {
	HAS_ARG(args);
	pari_sp ltop = avma;
	const char *order_s = (const char *)args->args;
	GEN order = strtoi(order_s);
	GEN e;
	if (min_d && min_curve == curve) {
		debug_log("Reusing min D = %Pi", min_d->d);
		// We have some discriminant, cannot use the roots because the D with walkdown was too large.
		if (min_d->d && isclone(min_d->d)) {
			gunclone(min_d->d);
		}
		if (min_d->p && isclone(min_d->p)) {
			gunclone(min_d->p);
		}
		good_qdisc_minimal(min_d, order);
		min_d->d = gclone(min_d->d);
		min_d->p = gclone(min_d->p);
		debug_log("Got min D = %Pi", min_d->d);
		if (min_roots) {
			cm_update_roots(min_d->d, min_d->p, min_roots);
		} else {
			min_roots = cm_make_roots(min_d->d, min_d->p);
		}
		e = cm_construct_curve(order, min_d->d, min_d->p, min_roots, false);
	} else {
		// We have nothing. Start fresh.
		debug_log("Fresh start.");
		if (!min_d) {
			min_d = try_calloc(sizeof(cm_any_qdisc_t));
		}
		if (min_d->d && isclone(min_d->d)) {
			gunclone(min_d->d);
		}
		if (min_d->p && isclone(min_d->p)) {
			gunclone(min_d->p);
		}
		good_qdisc_minimal(min_d, order);
		min_d->d = gclone(min_d->d);
		min_d->p = gclone(min_d->p);
		debug_log("Got min D = %Pi", min_d->d);
		min_roots = cm_make_roots(min_d->d, min_d->p);
		min_curve = curve;
		e = cm_construct_curve(order, min_d->d, min_d->p, min_roots, false);
	}

	if (e == NULL) {
		fprintf(err, "Could not construct curve.");
		avma = ltop;
		return -3;
	}

	GEN group = ellff_get_group(e);
	long len = glength(group);
	if (len == 2) {
		pari_sp btop = avma;
		debug_log("Walking down with structure %Ps.", group);
		GEN d = gcopy(min_d->d);
		GEN one = gel(group, 1);
		GEN two = gel(group, 2);
		GEN towalk = cmpii(one, two) < 0 ? one : two;
		GEN factored = Z_factor(towalk);
		GEN primes = gel(factored, 1);
		GEN powers = gel(factored, 2);
		for (long i = 1; i <= glength(primes); ++i) {
			GEN prime = gel(primes, i);
			GEN power = gel(powers, i);
			debug_log("Walking down with %Pi^(2*%Pi)", prime, power);
			GEN exp = mulis(power, 2);
			GEN mul = powii(prime, exp);
			debug_log("%Pi^%Pi = %Pi", prime, exp, mul);
			d = mulii(d, mul);
			debug_log("d = %Pi", d);
		}
		long dsize = logint(d, gen_2);
		if (dsize > 30) {
            debug_log("Discriminant too large after walkdown (d = %Pi, %li bits).", d, dsize);
            avma = ltop;
            return -3;
        }
		if (min_roots) {
			cm_update_roots(d, min_d->p, min_roots);
		} else {
			min_roots = cm_make_roots(d, min_d->p);
		}
		e = cm_construct_curve(order, d, min_d->p, min_roots, false);
		e = gerepilecopy(btop, e);
	}

	if (e == NULL) {
		fprintf(err, "Could not construct curve.");
		avma = ltop;
		return -3;
	}

	curve->field = gcopy(min_d->p);
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

void cm_any_quit() {
	if (min_d) {
		if (min_d->d && isclone(min_d->d)) {
			gunclone(min_d->d);
		}
		if (min_d->p && isclone(min_d->p)) {
			gunclone(min_d->p);
		}
		try_free(min_d);
	}
	if (min_roots) {
		cm_free_roots(min_roots);
	}
}
