/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <misc/types.h>
#include "brainpool.h"
#include "gen/seed.h"
#include "io/output.h"
#include "util/bits.h"
#include "util/str.h"

static seed_t *brainpool_new() {
	seed_t *result = seed_new();
	result->type = SEED_BRAINPOOL;
	return result;
}

static void seed_wv(seed_t *seed) {
	pari_sp ltop = avma;
	GEN L = utoi(cfg->bits);
	seed->brainpool.v = itou(gfloor(gdivgs(subis(L, 1), 160)));
	seed->brainpool.w =
			itou(subis(subis(L, 160 * seed->brainpool.v), 1));
	avma = ltop;
}

static void brainpool_update_seed(bits_t *s) {
	pari_sp ltop = avma;
	GEN z = bits_to_i(s);
	GEN t = Fp_add(z, gen_1, int2n(160));
	bits_t *result = bits_from_i_len(t, 160);
	avma = ltop;
	bits_cpy(s, result);
	bits_free(&result);
}

static bits_t *brainpool_hash(const bits_t *s, long w, long v) {
	pari_sp ltop = avma;
	unsigned char h0[20];
	bits_sha1(s, h0);
	unsigned char hashout[w + 20 * v];
	memcpy(hashout, h0, (size_t) w);

	GEN z = bits_to_i(s);
	GEN m = int2n(160);
	for (long i = 1; i < v; ++i) {
		bits_t *si = bits_from_i(Fp_add(z, stoi(i), m));
		bits_sha1(si, hashout + w + 20 * i);
		bits_free(&si);
	}
	bits_t *result = bits_from_raw(hashout, (size_t) (w + (20 * v)));
	avma = ltop;
	return result;
}

bool brainpool_seed_valid(const char *hex_str) {
	const char *seed = str_is_hex(hex_str);
	return seed && strlen(seed) == 40;
}

GENERATOR(brainpool_gen_seed_random) {
	seed_t *seed = brainpool_new();
	seed->seed = bits_new_rand(160);
	seed_wv(seed);
	curve->seed = seed;
	return 1;
}

GENERATOR(brainpool_gen_seed_argument) {
	seed_t *seed = brainpool_new();
	seed->seed = bits_from_hex(str_is_hex(cfg->seed));
	seed_wv(seed);
	curve->seed = seed;
	return 1;
}

GENERATOR(brainpool_gen_seed_input) {
	pari_sp ltop = avma;

	GEN str = input_string("seed:");
	const char *cstr = GSTR(str);
	if (!brainpool_seed_valid(cstr)) {
		fprintf(err, "SEED must be exactly 160 bits(40 hex characters).\n");
		avma = ltop;
		return 0;
	}

	seed_t *seed = brainpool_new();
	seed->seed = bits_from_hex(str_is_hex(cstr));
	seed_wv(seed);
	curve->seed = seed;
	return 1;
}

GENERATOR(brainpool_gen_field) {
	pari_sp btop = avma;
	seed_t *seed = curve->seed;
	do {
		if (seed->brainpool.update_seed) {
			brainpool_update_seed(seed->seed);
			seed->brainpool.update_seed = false;
		}
		bits_t *p_bits = brainpool_hash(seed->seed, seed->brainpool.w + 1, seed->brainpool.v);
		GEN c = bits_to_i(p_bits);
		bits_free(&p_bits);
		GEN p = c;
		do {
			p = nextprime(p);
		} while (mod4(p) != 3);

		long p_len = glength(binary_zv(p));
		if (p_len >= cfg->bits || p_len <= cfg->bits - 1) {
			brainpool_update_seed(seed->seed);
			avma = btop;
			continue;
		}

		if (!isprime(p)) {
			brainpool_update_seed(seed->seed);
			avma = btop;
			continue;
		}

		curve->field = p;
		gerepileall(btop, 1, &curve->field);
		break;
	} while (true);

	seed->brainpool.update_seed = true;
	return 1;
}

GENERATOR(brainpool_gen_equation) {
	// field is definitely prime
	pari_sp btop = avma;
	seed_t *seed = curve->seed;
	do {
		if (seed->brainpool.update_seed) {
			brainpool_update_seed(seed->seed);
			seed->brainpool.update_seed = false;
		}

		GEN z;
		bits_t *a_bits =
				brainpool_hash(seed->seed, seed->brainpool.w, seed->brainpool.v);
		GEN a = bits_to_i(a_bits);
		bits_free(&a_bits);
		z = Fp_sqrtn(a, stoi(4), curve->field, NULL);
		if (z == NULL) {
			brainpool_update_seed(seed->seed);
			avma = btop;
			continue;
		}
		seed->brainpool.seed_a = bits_copy(seed->seed);

		brainpool_update_seed(seed->seed);

		bits_t *b_bits =
				brainpool_hash(seed->seed, seed->brainpool.w, seed->brainpool.v);
		GEN b = bits_to_i(b_bits);
		bits_free(&b_bits);
		if (!Fp_issquare(b, curve->field)) {
			brainpool_update_seed(seed->seed);
			bits_free(&seed->brainpool.seed_a);
			avma = btop;
			continue;
		}
		seed->brainpool.seed_b = bits_copy(seed->seed);

		GEN mod_a = gmodulo(a, curve->field);
		GEN mod_b = gmodulo(b, curve->field);

		if (gequal0(gmulsg(-16, gadd(gmulsg(4, gpowgs(mod_a, 3)),
									 gmulsg(27, gsqr(mod_b)))))) {
			brainpool_update_seed(seed->seed);
			bits_free(&seed->brainpool.seed_a);
			bits_free(&seed->brainpool.seed_b);
			avma = btop;
			continue;
		}

		brainpool_update_seed(seed->seed);
		seed->brainpool.seed_bp = bits_copy(seed->seed);

		bits_t *mult_bits =
				brainpool_hash(seed->seed, seed->brainpool.w, seed->brainpool.v);
		seed->brainpool.mult = bits_to_i(mult_bits);

		curve->a = mod_a;
		curve->b = mod_b;
		gerepileall(btop, 2, &curve->a, &curve->b);
		break;
	} while (true);

	seed->brainpool.update_seed = true;
	return 1;
}