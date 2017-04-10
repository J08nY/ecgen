/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "order.h"

int order_any(curve_t *curve, const config_t *cfg, arg_t *args) {
	GEN ord = ellff_get_card(curve->curve);
	if (isclone(ord)) {
		curve->order = gcopy(ord);
	} else {
		curve->order = ord;
	}
	return 1;
}

int order_smallfact(curve_t *curve, const config_t *cfg, arg_t *args) {
	if (!args) {
		fprintf(stderr, "No args to an arged function. order_smallfact");
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
		obj_insert(curve->curve, 1, curve->order);
		return 1;
	}
}

int order_prime(curve_t *curve, const config_t *cfg, arg_t *args) {
	pari_sp ltop = avma;
	GEN order = ellsea(curve->curve, 1);
	if (gequal0(order) || !(isprime(order))) {
		avma = ltop;
		return -4;
	} else {
		curve->order = order;
		obj_insert(curve->curve, 1, curve->order);
		return 1;
	}
}
