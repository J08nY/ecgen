/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "order.h"

GEN order_factors(curve_t *curve, const config_t *cfg) {
	if (cfg->prime) {
		return gtovec(curve->order);
	} else {
		GEN factors = Z_factor(curve->order);
		return gel(factors, 1);
	}
}

GEN order_groups(curve_t *curve, const config_t *cfg, GEN factors) {
	long nprimes = glength(factors);
	if (cfg->prime) {
		return gtovec(curve->order);
	} else {
		GEN amount = int2n(nprimes);
		GEN groups = gtovec0(gen_0, itos(amount) - 1);

		for (size_t count = 1; count < (size_t)(1 << nprimes); ++count) {
			GEN result = gen_1;
			for (long bit = 0; bit < nprimes; ++bit) {
				size_t mask = (size_t)(1 << bit);
				if (count & mask) {
					result = mulii(result, gel(factors, bit + 1));
				}
			}
			gel(groups, count) = result;
		}
		// TODO: sort this, as it is not necessarily sorted, in fact most likely
		// not
		return groups;
	}
}

GENERATOR(order_gen_any) {
	GEN ord = ellff_get_card(curve->curve);
	if (isclone(ord)) {
		curve->order = gcopy(ord);
	} else {
		curve->order = ord;
	}
	return 1;
}

GENERATOR(order_gen_sea) {
	pari_sp ltop = avma;
	GEN order = ellsea(curve->curve, 0);
	if (gequal0(order)) {
		avma = ltop;
		return -4;
	} else {
		curve->order = order;
		obj_insert_shallow(curve->curve, 1, order);
		return 1;
	}
}

GENERATOR(order_gen_smallfact) {
	if (!args) {
		fprintf(stderr, "No args to an arged function. order_gen_smallfact\n");
		return INT_MIN;
	}

	pari_ulong smallfact = *(pari_ulong *)args->args;
	pari_sp ltop = avma;
	GEN fact = mpfact(smallfact);
	if (lgefint(fact) > 3) {
		smallfact = 0;
	} else {
		smallfact = itou(fact);
	}

	GEN order = ellsea(curve->curve, smallfact);
	if (gequal0(order) || gequal1(gcdii(order, fact))) {
		avma = ltop;
		return -4;
	} else {
		curve->order = order;
		obj_insert_shallow(curve->curve, 1, curve->order);
		return 1;
	}
}

GENERATOR(order_gen_prime) {
	pari_sp ltop = avma;
	GEN order = ellsea(curve->curve, 1);
	if (gequal0(order) || !(isprime(order))) {
		avma = ltop;
		return -4;
	} else {
		curve->order = order;
		obj_insert_shallow(curve->curve, 1, curve->order);
		return 1;
	}
}
