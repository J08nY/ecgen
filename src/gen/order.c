/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "order.h"
#include <misc/types.h>
#include "exhaustive/arg.h"
#include "io/input.h"

GENERATOR(order_gen_input) {
	pari_sp ltop = avma;
	GEN ord = input_int("order", cfg->bits);
	if (gequalm1(ord)) {
		avma = ltop;
		return -4;
	} else {
		curve->order = ord;
		obj_insert_shallow(curve->curve, 1, ord);
		return 1;
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
	HAS_ARG(args);
	pari_ulong smallfact = *(pari_ulong *)args->args;
	pari_sp ltop = avma;
	GEN fact = mpfact(smallfact);
	pari_ulong lfact = 0;
	if (lgefint(fact) > 3) {
		lfact = 0;
	} else {
		lfact = itou(fact);
	}

	GEN order = ellsea(curve->curve, lfact);
	if (gequal0(order)) {
		avma = ltop;
		return -4;
	}

	GEN factors = factor(order);
	GEN primes = gel(factors, 1);
	GEN powers = gel(factors, 2);
	long len = glength(primes);
	GEN total = gen_1;
	for (long i = 1; i < len; ++i) {
		GEN pow = powii(gel(primes, i), gel(powers, i));
		total = mulii(total, pow);
		if (abscmpiu(total, smallfact) > 0) {
			avma = ltop;
			return -4;
		}
	}

	curve->order = gerepileupto(ltop, order);
	obj_insert_shallow(curve->curve, 1, curve->order);
	return 1;
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

CHECK(order_check_pohlig_hellman) {
	HAS_ARG(args);
	pari_sp ltop = avma;

	const char *min_fact = args->args;
	GEN minf = strtoi(min_fact);

	GEN factors = factor(curve->order);
	GEN primes = gel(factors, 1);

	long len = glength(primes);
	if (mpcmp(gel(primes, len), minf) <= 0) {
		avma = ltop;
		return -4;
	} else {
		avma = ltop;
		return 1;
	}
}

CHECK(order_check_discriminant) {
	HAS_ARG(args);
	if (cfg->field == FIELD_BINARY) return 1;
	pari_sp ltop = avma;

	const char *min_disc = args->args;
	GEN mind = strtoi(min_disc);

	GEN t = negi(subii(curve->order, addii(curve->field, gen_1)));
	GEN tp = subii(sqri(t), mulis(curve->field, 4));
	GEN tp_factors = factor(tp);

	GEN tp_primes = gel(tp_factors, 1);
	GEN tp_pows = gel(tp_factors, 2);
	long tp_pow_len = glength(tp_pows);
	GEN max_value = gen_1;
	for (long i = 1; i <= tp_pow_len; ++i) {
		if (!dvdis(gel(tp_pows, i), 2)) {
			continue;
		}

		GEN value = powii(gel(tp_primes, i), gel(tp_pows, i));
		if (mpcmp(max_value, value) < 0) {
			max_value = value;
		}
	}
	GEN s = max_value;

	GEN D = divii(tp, s);
	if (mod4(D) != 1) {
		D = mulis(D, 4);
	}

	if (mpcmp(D, mind) <= 0) {
		avma = ltop;
		return -4;
	}
	avma = ltop;
	return 1;
}