/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "order.h"
#include "exhaustive/arg.h"
#include "field.h"
#include "io/input.h"
#include "math/koblitz.h"

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
	pari_sp ltop = avma;
	GEN ord = ellff_get_card(curve->curve);
	if (isclone(ord)) {
		curve->order = gerepilecopy(ltop, ord);
	} else {
		avma = ltop;
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
		curve->order = gerepilecopy(ltop, order);
		obj_insert_shallow(curve->curve, 1, order);
		return 1;
	}
}

GENERATOR(order_gen_cofactor) {
	HAS_ARG(args);
	pari_ulong cofactor = *(pari_ulong *)args->args;
	pari_sp ltop = avma;
	GEN order = ellff_get_card(curve->curve);
	if (!dvdii(order, utoi(cofactor))) {
		avma = ltop;
		return -4;
	}
	GEN res = diviuexact(order, cofactor);
	if (!isprime(res)) {
		avma = ltop;
		return -4;
	}

	curve->order = gerepilecopy(ltop, order);
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
		curve->order = gerepilecopy(ltop, order);
		obj_insert_shallow(curve->curve, 1, curve->order);
		return 1;
	}
}

GENERATOR(order_gen_koblitz) {
	pari_sp ltop = avma;
	GEN order = koblitz_get_order(cfg->bits, itou(curve->a));
	if (order) {
		curve->order = gerepilecopy(ltop, order);
		obj_insert_shallow(curve->curve, 1, curve->order);
	} else {
		GEN ord = ellff_get_card(curve->curve);
		if (isclone(ord)) {
			curve->order = gerepilecopy(ltop, ord);
		} else {
			avma = ltop;
			curve->order = ord;
		}
	}
	return 1;
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

	GEN D = field_elementi(core(tp));
	if (abscmpii(D, mind) <= 0) {
		avma = ltop;
		return -4;
	}
	avma = ltop;
	return 1;
}