/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "curve.h"
#include "field.h"
#include "seed.h"

curve_t *curve_new() {
	curve_t *curve = pari_malloc(sizeof(curve_t));
	if (!curve) {
		perror("Couldn't malloc.");
		exit(1);
	}
	memset(curve, 0, sizeof(curve_t));
	return curve;
}

void curve_free(curve_t **curve) {
	if (*curve) {
		seed_free(&(*curve)->seed);
		pari_free((*curve)->points);
		pari_free(*curve);
		*curve = NULL;
	}
}

int curve_init(curve_t *curve, config_t *config) {
	pari_sp ltop = avma;
	GEN v = gen_0;
	switch (typ(curve->field)) {
		case t_INT:
			v = gtovec0(gen_0, 2);
			gel(v, 1) = curve->a;
			gel(v, 2) = curve->b;
			break;
		case t_FFELT:
			v = gtovec0(gen_0, 5);
			gel(v, 1) = gen_1;
			gel(v, 4) = curve->a;
			gel(v, 5) = curve->b;
			break;
		default:
			pari_err_TYPE("curve_init", curve->field);
	}

	curve->curve = gerepilecopy(ltop, ellinit(v, curve->field, -1));
	return 1;
}

int curve_nonzero(curve_t *curve, config_t *config) {
	pari_sp ltop = avma;
	curve_init(curve, config);
	if (gequal0(ell_get_disc(curve->curve))) {
		avma = ltop;
		return -3;
	} else {
		return 1;
	}
}

int curve_prime(curve_t *curve, config_t *config) {
	pari_sp ltop = avma;
	int nonzero = curve_nonzero(curve, config);
	if (nonzero == 1) {
		curve->order = ellsea(curve->curve, 1);
		if (gequal0(curve->order) || !(isprime(curve->order))) {
			avma = ltop;
			return -3;
		} else {
			return 1;
		}
	} else {
		avma = ltop;
		return nonzero;
	}
}

int curve_seed_fp(curve_t *curve, config_t *config) {}

int curve_seed_f2m(curve_t *curve, config_t *config) {}

int curve_seed(curve_t *curve, config_t *config) {
	switch (typ(curve->field)) {
		case t_INT:
			return curve_seed_fp(curve, config);
		case t_FFELT:
			return curve_seed_f2m(curve, config);
		default:
			pari_err_TYPE("curve_seed", curve->field);
			return 0; /* NOT REACHABLE */
	}
}

int curve_g(curve_t *curve, config_t *config) {
	if (config->from_seed) {
		return curve_seed(curve, config);
	} else if (config->prime) {
		return curve_prime(curve, config);
	} else {
		return curve_nonzero(curve, config);
	}
}

GEN curve_params(curve_t *curve) {
	pari_sp ltop = avma;

	GEN result = field_params(curve->field);
	if (curve->a) result = gconcat(result, field_elementi(curve->a));
	if (curve->b) result = gconcat(result, field_elementi(curve->b));
	if (curve->order) result = gconcat(result, gtovec(curve->order));

	return gerepilecopy(ltop, result);
}
