/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "order.h"
#include "exhaustive/arg.h"
#include "io/input.h"
#include "io/output.h"

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
