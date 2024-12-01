/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2024 J08nY
 */

#include "family.h"
#include "cm/cm_any.h"
#include "gen/seed.h"
#include "misc/config.h"
#include "util/random.h"

#define FAMILIES (FAMILY_KSS40 + 1)

static GEN nz_store[FAMILIES] = {0};
static GEN pz_store[FAMILIES] = {0};
static GEN rz_store[FAMILIES] = {0};
static GEN tz_store[FAMILIES] = {0};
static GEN D_store[FAMILIES] = {0};

void family_init() {
	pari_sp ltop = avma;
	nz_store[FAMILY_BN] = gclone(closure_evalgen(compile_str("(z) -> z")));
	pz_store[FAMILY_BN] = gclone(closure_evalgen(
	    compile_str("(z) -> 36*z^4 + 36*z^3 + 24*z^2 + 6*z + 1")));
	rz_store[FAMILY_BN] = gclone(closure_evalgen(
	    compile_str("(z) -> 36*z^4 + 36*z^3 + 18*z^2 + 6*z + 1")));
	tz_store[FAMILY_BN] =
	    gclone(closure_evalgen(compile_str("(z) -> 6*z + 1")));
	D_store[FAMILY_BN] = gclone(stoi(-3));

	nz_store[FAMILY_BLS12] = gclone(closure_evalgen(compile_str("(z) -> z")));
	pz_store[FAMILY_BLS12] = gclone(closure_evalgen(
	    compile_str("(z) -> (z - 1)^2 * (z^4 - z^2 + 1)/3 + z")));
	rz_store[FAMILY_BLS12] =
	    gclone(closure_evalgen(compile_str("(z) -> z^4 - z^2 + 1")));
	tz_store[FAMILY_BLS12] =
	    gclone(closure_evalgen(compile_str("(z) -> z + 1")));
	D_store[FAMILY_BLS12] = gclone(stoi(-3));

	nz_store[FAMILY_BLS24] = gclone(closure_evalgen(compile_str("(z) -> z")));
	pz_store[FAMILY_BLS24] = gclone(closure_evalgen(
	    compile_str("(z) -> (z - 1)^2 * (z^8 - z^4 + 1)/3 + z")));
	rz_store[FAMILY_BLS24] =
	    gclone(closure_evalgen(compile_str("(z) -> z^8 - z^4 + 1")));
	tz_store[FAMILY_BLS24] =
	    gclone(closure_evalgen(compile_str("(z) -> z + 1")));
	D_store[FAMILY_BLS24] = gclone(stoi(-3));

	//TODO: This does not work...
	nz_store[FAMILY_KSS16] =
	    gclone(closure_evalgen(compile_str("(z) -> 70*z + 25")));
	pz_store[FAMILY_KSS16] = gclone(closure_evalgen(
	    compile_str("(z) -> (z^10 + 2*z^9 + 5*z^8 + 48*z^6 + 152*z^5 + 240*z^4 "
	                "+ 625*z^2 + 2398*z + 3125)/980")));
	rz_store[FAMILY_KSS16] = gclone(
	    closure_evalgen(compile_str("(z) -> (z^8 + 48*z^4 + 625)/61250")));
	tz_store[FAMILY_KSS16] =
	    gclone(closure_evalgen(compile_str("(z) -> (2*z^5 + 41*z + 35)/35")));
	D_store[FAMILY_KSS16] = gclone(stoi(-1));

	avma = ltop;
}

static seed_t *family_new_seed() {
	seed_t *result = seed_new();
	result->type = SEED_FAMILY;
	return result;
}

GENERATOR(family_gen_seed_random) {
	curve->seed = family_new_seed();
	curve->seed->family.z = random_int(cfg->bits);
	return 1;
}

GENERATOR(family_gen_seed_input) {
	pari_sp ltop = avma;
	GEN inp = input_int("z:", cfg->bits);
	if (gequalm1(inp)) {
		avma = ltop;
		return 0;
	} else if (equalii(inp, gen_m2)) {
		avma = ltop;
		return INT_MIN;
	}
	curve->seed = family_new_seed();
	curve->seed->family.z = inp;
	return 1;
}

GENERATOR(family_gen_field) {
	pari_sp ltop = avma;
	GEN n = closure_callgen1(nz_store[cfg->family], curve->seed->family.z);
	GEN pz = closure_callgen1(pz_store[cfg->family], n);
	if (typ(pz) != t_INT || !isprime(pz)) {
		avma = ltop;
		return -1;
	}
	printf("p");
	GEN rz = closure_callgen1(rz_store[cfg->family], n);
	if (typ(rz) != t_INT || !isprime(rz)) {
		avma = ltop;
		return -1;
	}
	printf("r");
	curve->field = gerepilecopy(ltop, pz);
	return 1;
}

static GEN b = NULL;
static curve_t *b_curve = NULL;

GENERATOR(family_gen_equation_iter) {
	curve->a = gmodulo(gen_0, curve->field);

	pari_sp ltop = avma;
	if (!b) {
		b = gclone(gen_1);
		curve->b = gmodulo(gen_1, curve->field);
		b_curve = curve;
		return 1;
	} else {
		if (curve == b_curve) {
			GEN bn = addii(b, gen_1);
			gunclone(b);
			b = gclone(bn);
			curve->b = gerepilecopy(ltop, gmodulo(bn, curve->field));
			return 1;
		} else {
			// dont use b, regenerate it,
			gunclone(b);
			b = gclone(gen_1);
			curve->b = gerepilecopy(ltop, gmodulo(gen_1, curve->field));
			b_curve = curve;
			return 1;
		}
	}
}

GENERATOR(family_gen_equation_cm) {
	GEN n = closure_callgen1(nz_store[cfg->family], curve->seed->family.z);
	GEN rz = closure_callgen1(rz_store[cfg->family], n);
	GEN D = D_store[cfg->family];
	GEN e = cm_construct_curve_subgroup(rz, D, curve->field);
	if (e) {
		curve->a = ell_get_a4(e);
		curve->b = ell_get_a6(e);
		return 1;
	}
	return -3;
}

GENERATOR(family_gen_order) {
	pari_sp ltop = avma;
	GEN n = closure_callgen1(nz_store[cfg->family], curve->seed->family.z);
	GEN rz = closure_callgen1(rz_store[cfg->family], n);
	GEN ord = ellff_get_card(curve->curve);
	if (dvdii(ord, rz)) {
		if (isclone(ord)) {
			curve->order = gerepilecopy(ltop, ord);
		} else {
			avma = ltop;
			curve->order = ord;
		}
		return 1;
	} else {
		avma = ltop;
		return -2;
	}
}

void family_quit() {
	for (int i = 0; i < FAMILIES; i++) {
		if (nz_store[i]) {
			gunclone(nz_store[i]);
		}
		if (pz_store[i]) {
			gunclone(pz_store[i]);
		}
		if (rz_store[i]) {
			gunclone(rz_store[i]);
		}
		if (tz_store[i]) {
			gunclone(tz_store[i]);
		}
		if (D_store[i]) {
			gunclone(D_store[i]);
		}
	}
	if (b) {
		gunclone(b);
	}
}