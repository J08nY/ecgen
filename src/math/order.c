/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "order.h"

int order_any(curve_t *curve, config_t *cfg, arg_t *args) {
	curve->order = ellff_get_card(curve->curve);
	return 1;
}

int order_smallfact(curve_t *curve, config_t *cfg, arg_t *args) {
	if (!args) {
		fprintf(stderr, "No args to an arged function. order_smallfact");
		return INT_MIN;
	}
	pari_ulong smallfact = *(pari_ulong *)args->args;
	pari_sp ltop = avma;
	curve->order = ellsea(curve->curve, smallfact);
	obj_insert_shallow(curve->curve, 1, curve->order);
	if (gequal0(curve->order)) {
		avma = ltop;
		return -4;
	} else {
		return 1;
	}
}

int order_prime(curve_t *curve, config_t *cfg, arg_t *args) {
	pari_sp ltop = avma;
	curve->order = ellsea(curve->curve, 1);
	obj_insert_shallow(curve->curve, 1, curve->order);
	if (gequal0(curve->order) || !(isprime(curve->order))) {
		avma = ltop;
		return -4;
	} else {
		return 1;
	}
}
