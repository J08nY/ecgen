/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "order.h"

int order_init(curve_t *curve, config_t *cfg, ...) {
	curve->order = ellff_get_card(curve->curve);
	return 1;
}

int order_prime(curve_t *curve, config_t *cfg, ...) {
	pari_sp ltop = avma;
	curve->order = ellsea(curve->curve, 1);
	if (gequal0(curve->order) || !(isprime(curve->order))) {
		avma = ltop;
		return -4;
	} else {
		return 1;
	}
}