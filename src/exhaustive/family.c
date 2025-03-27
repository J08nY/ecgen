/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2024 J08nY
 */

#include "family.h"
#include "cm/cm_any.h"
#include "gen/seed.h"
#include "io/output.h"
#include "misc/config.h"
#include "util/bits.h"
#include "util/random.h"

#define FAMILIES (FAMILY_KSS40 + 1)

static GEN nz_store[FAMILIES] = {0};
static GEN pz_store[FAMILIES] = {0};
static GEN rz_store[FAMILIES] = {0};
static GEN tz_store[FAMILIES] = {0};
static GEN D_store[FAMILIES] = {0};

// clang-format off
void family_init(void) {
	pari_sp ltop = avma;
	nz_store[FAMILY_BN] = gclone(closure_evalgen(compile_str("(z) -> z")));
	pz_store[FAMILY_BN] = gclone(closure_evalgen(compile_str("(z) -> 36*z^4 + 36*z^3 + 24*z^2 + 6*z + 1")));
	rz_store[FAMILY_BN] = gclone(closure_evalgen(compile_str("(z) -> 36*z^4 + 36*z^3 + 18*z^2 + 6*z + 1")));
	tz_store[FAMILY_BN] = gclone(closure_evalgen(compile_str("(z) -> 6*z + 1")));
	D_store[FAMILY_BN] = gclone(stoi(-3));

	nz_store[FAMILY_BLS12] = gclone(closure_evalgen(compile_str("(z) -> z")));
	pz_store[FAMILY_BLS12] = gclone(closure_evalgen(compile_str("(z) -> (z - 1)^2 * (z^4 - z^2 + 1)/3 + z")));
	rz_store[FAMILY_BLS12] = gclone(closure_evalgen(compile_str("(z) -> z^4 - z^2 + 1")));
	tz_store[FAMILY_BLS12] = gclone(closure_evalgen(compile_str("(z) -> z + 1")));
	D_store[FAMILY_BLS12] = gclone(stoi(-3));

	nz_store[FAMILY_BLS24] = gclone(closure_evalgen(compile_str("(z) -> z")));
	pz_store[FAMILY_BLS24] = gclone(closure_evalgen(compile_str("(z) -> (z - 1)^2 * (z^8 - z^4 + 1)/3 + z")));
	rz_store[FAMILY_BLS24] = gclone(closure_evalgen(compile_str("(z) -> z^8 - z^4 + 1")));
	tz_store[FAMILY_BLS24] = gclone(closure_evalgen(compile_str("(z) -> z + 1")));
	D_store[FAMILY_BLS24] = gclone(stoi(-3));

	nz_store[FAMILY_KSS16] = gclone(closure_evalgen(compile_str("(z) -> 70*z + 25")));
	pz_store[FAMILY_KSS16] = gclone(closure_evalgen(compile_str("(z) -> (z^10 + 2*z^9 + 5*z^8 + 48*z^6 + 152*z^5 + 240*z^4 + 625*z^2 + 2398*z + 3125)/980")));
	rz_store[FAMILY_KSS16] = gclone(closure_evalgen(compile_str("(z) -> (z^8 + 48*z^4 + 625)/61250")));
	tz_store[FAMILY_KSS16] = gclone(closure_evalgen(compile_str("(z) -> (2*z^5 + 41*z + 35)/35")));
	D_store[FAMILY_KSS16] = gclone(stoi(-4));

	nz_store[FAMILY_KSS18] = gclone(closure_evalgen(compile_str("(z) -> 42*z + 14")));
	pz_store[FAMILY_KSS18] = gclone(closure_evalgen(compile_str("(z) -> (z^8 + 5*z^7 + 7*z^6 + 37*z^5 + 188*z^4 + 259*z^3 + 343*z^2 + 1763*z + 2401)/21")));
	rz_store[FAMILY_KSS18] = gclone(closure_evalgen(compile_str("(z) -> (z^6 + 37*z^3 + 343)/343")));
	tz_store[FAMILY_KSS18] = gclone(closure_evalgen(compile_str("(z) -> (z^4 + 16*z + 7)/7")));
	D_store[FAMILY_KSS18] = gclone(stoi(-3));

	nz_store[FAMILY_KSS36] = gclone(closure_evalgen(compile_str("(z) -> 777 * z + 287")));
	pz_store[FAMILY_KSS36] = gclone(closure_evalgen(compile_str("(z) -> (z^14 - 4*z^13 + 7*z^12 + 683*z^8 - 2510*z^7 + 4781*z^6 + 117649*z^2 - 386569*z + 823543)/28749")));
	rz_store[FAMILY_KSS36] = gclone(closure_evalgen(compile_str("(z) -> (z^12 + 683*z^6 + 117649)/161061481")));
	tz_store[FAMILY_KSS36] = gclone(closure_evalgen(compile_str("(z) -> (2*z^7 + 757*z + 259)/259")));
	D_store[FAMILY_KSS36] = gclone(stoi(-3));

	nz_store[FAMILY_KSS40] = gclone(closure_evalgen(compile_str("(z) -> 2370*z + 1205")));
	pz_store[FAMILY_KSS40] = gclone(closure_evalgen(compile_str("(z) -> (z^22 - 2*z^21 + 5*z^20 + 6232*z^12 - 10568*z^11 + 31160*z^10 + 9765625*z^2 - 13398638*z + 48828125)/1123380")));
	rz_store[FAMILY_KSS40] = gclone(closure_evalgen(compile_str("(z) -> (z^16 + 8*z^14 + 39*z^12 + 112*z^10 - 79*z^8 + 2800*z^6 + 24375*z^4 + 125000*z^2 + 390625)/2437890625")));
	tz_store[FAMILY_KSS40] = gclone(closure_evalgen(compile_str("(z) -> (2*z^11 + 6469*z + 1185)/1185")));
	D_store[FAMILY_KSS40] = gclone(stoi(-4));

	avma = ltop;
}
// clang-format on

static seed_t *family_new_seed(void) {
	seed_t *result = seed_new();
	result->type = SEED_FAMILY;
	return result;
}

GENERATOR(family_gen_seed_random) {
	curve->seed = family_new_seed();
	curve->seed->family.z = random_int(cfg->bits);
	if (random_bits(1)) {
		togglesign(curve->seed->family.z);
	}
	curve->seed->seed = bits_from_i(curve->seed->family.z);
	return 1;
}

GENERATOR(family_gen_seed_input) {
	GEN inp = input_int("z:", cfg->bits);
	curve->seed = family_new_seed();
	curve->seed->family.z = inp;
	curve->seed->seed = bits_from_i(curve->seed->family.z);
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
	GEN rz = closure_callgen1(rz_store[cfg->family], n);
	if (typ(rz) != t_INT || !isprime(rz)) {
		avma = ltop;
		return -1;
	}
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
	cm_any_roots_t *roots = cm_make_roots(D, curve->field);
	GEN e = cm_construct_curve_subgroup(rz, D, curve->field, roots);
	cm_free_roots(roots);
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

void family_quit(void) {
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
