/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "curve.h"

curve_t *new_curve() {
	curve_t *curve = malloc(sizeof(curve_t));
	if (!curve) {
		perror("Couldn't malloc.");
		exit(1);
	}
	memset(curve, 0, sizeof(curve_t));
	return curve;
}

void free_curve(curve_t **curve) {
	if (*curve) {
		free((*curve)->points);
		free(*curve);
		*curve = NULL;
	}
}

curve_t *curve_random(GEN field) {
	curve_t *result = new_curve();
	pari_sp ltop = avma;

	GEN curve, a, b;
	a = genrand(field);
	b = genrand(field);

	GEN v = gen_0;
	switch (typ(field)) {
		case t_INT:
			v = gtovec0(gen_0, 2);
			gel(v, 1) = a;
			gel(v, 2) = b;
			break;
		case t_FFELT:
			v = gtovec0(gen_0, 5);
			gel(v, 1) = gen_1;
			gel(v, 4) = a;
			gel(v, 5) = b;
			break;
		default:
			pari_err_TYPE("curve_random", field);
	}
	curve = ellinit(v, field, -1);

	result->field = field;
	gerepileall(ltop, 3, &a, &b, &curve);
	result->a = a;
	result->b = b;
	result->curve = curve;
	return result;
}

curve_t *curve_randomf(enum field_e t, long bits) {
	GEN field = field_random(t, bits);
	return curve_random(field);
}

curve_t *curve_nonzero(GEN field) {
	pari_sp ltop = avma;

	curve_t *result = NULL;
	do {
		if (result) {
			avma = ltop;
			free_curve(&result);
		}
		result = curve_random(field);
	} while (gequal0(ell_get_disc(result->curve)));

	return result;
}

curve_t *curve_nonzerof(enum field_e t, long bits) {
	pari_sp ltop = avma;

	curve_t *result = NULL;
	do {
		if (result) {
			avma = ltop;
			free_curve(&result);
		}
		result = curve_randomf(t, bits);
	} while(gequal0(ell_get_disc(result->curve)));

	return result;
}

curve_t *curve_prime(GEN field) {
	pari_sp ltop = avma;

	curve_t *result = NULL;
	do {
		if (result) {
			avma = ltop;
			free_curve(&result);
		}
		result = curve_nonzero(field);
		result->order = ellsea(result->curve, 1);
	} while (gequal0(result->order) || !isprime(result->order));

	return result;
}

curve_t *curve_primef(enum field_e t, long bits) {
	pari_sp ltop = avma;

	curve_t *result = NULL;
	do {
		if (result) {
			avma = ltop;
			free_curve(&result);
		}
		result = curve_nonzerof(t, bits);
		result->order = ellsea(result->curve, 1);
	} while (gequal0(result->order) || !isprime(result->order));

	return result;
}

GEN curve_invalid(GEN field, GEN a, GEN b) {
	return gen_m1;
}

curve_t *curve_seed_Fp(GEN field, GEN seed) {
	return NULL;
}

curve_t *curve_seed_F2m(GEN field, GEN seed) {
	return NULL;
}

curve_t *curve_seed(GEN field, GEN seed) {
	switch (typ(field)) {
		case t_INT:
			return curve_seed_Fp(field, seed);
		case t_FFELT:
			return curve_seed_F2m(field, seed);
		default:
			pari_err_TYPE("curve_seed", field);
			return NULL; /* NOT REACHED */
	}
}

curve_t *curve_seedr(GEN field) {
	pari_sp ltop = avma;

	GEN seed;
	curve_t *result = NULL;
	do {
		if (result) {
			avma = ltop;
			free_curve(&result);
		}
		seed = random_int(160);
		result = curve_seed(field, seed);
	} while(gequal0(ell_get_disc(result->curve)));

	return result;
}

curve_t *curve_seedf(GEN seed, enum field_e t, long bits) {
	pari_sp ltop = avma;

	GEN field;
	curve_t *result = NULL;
	do {
		if (result) {
			avma = ltop;
			free_curve(&result);
		}
		field = field_random(t, bits);
		result = curve_seed(field, seed);
	} while(gequal0(ell_get_disc(result->curve)));

	return result;
}

curve_t *curve_seedrf(enum field_e t, long bits) {
	pari_sp ltop = avma;

	GEN seed, field;
	curve_t *result = NULL;
	do {
		if (result) {
			avma = ltop;
			free_curve(&result);
		}
		seed = random_int(160);
		field = field_random(t, bits);
		result = curve_seed(field, seed);
	} while(gequal0(ell_get_disc(result->curve)));

	return result;
}

GEN curve_params(curve_t *curve) {
	pari_sp ltop = avma;

	GEN field = field_params(curve->field);
	GEN a = gtovec(field_elementi(curve->a));
	GEN b = gtovec(field_elementi(curve->b));

	return gerepilecopy(ltop, gconcat(gconcat(field, a), b));
}
