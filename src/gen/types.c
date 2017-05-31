/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "types.h"

int gen_skip(curve_t *curve, const config_t *cfg, arg_t *args) { return 1; }

int unroll_skip(curve_t *curve, const config_t *cfg, pari_sp from, pari_sp to) {
	return -1;
}
