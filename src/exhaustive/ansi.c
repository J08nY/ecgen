/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */

#include "ansi.h"
#include "gen/field.h"
#include "gen/seed.h"
#include "io/output.h"
#include "util/bits.h"
#include "util/memory.h"
#include "util/str.h"

static seed_t *ansi_new() {
	seed_t *result = seed_new();
	result->type = SEED_ANSI;
	return result;
}

bool ansi_seed_valid(const char *hex_str) {
	const char *seed = str_is_hex(hex_str);
	return seed && strlen(seed) >= 40;
}

static void seed_hash(seed_t *seed) {
	seed->hash20 = try_malloc(20);
	bits_sha1(seed->seed, seed->hash20);
}

static void seed_tsh(seed_t *seed) {
	pari_sp ltop = avma;
	seed->ansi.t = utoi(cfg->bits);
	seed->ansi.s =
	    floorr(rdivii(subis(seed->ansi.t, 1), stoi(160), DEFAULTPREC));
	seed->ansi.h = subii(seed->ansi.t, mulis(seed->ansi.s, 160));
	gerepileall(ltop, 3, &seed->ansi.t, &seed->ansi.s, &seed->ansi.h);
}

GENERATOR(ansi_gen_seed_random) {
	seed_t *seed = ansi_new();
	seed->seed = bits_from_i(random_int(160));
	seed_hash(seed);
	seed_tsh(seed);
	curve->seed = seed;
	return 1;
}

GENERATOR(ansi_gen_seed_argument) {
	seed_t *seed = ansi_new();
	seed->seed = bits_from_hex(str_is_hex(cfg->seed));
	seed_hash(seed);
	seed_tsh(seed);
	curve->seed = seed;
	return 1;
}

GENERATOR(ansi_gen_seed_input) {
	pari_sp ltop = avma;

	GEN str = input_string("seed:");
	const char *cstr = GSTR(str);
	if (!ansi_seed_valid(cstr)) {
		fprintf(err, "SEED must be at least 160 bits(40 hex characters).\n");
		avma = ltop;
		return 0;
	}

	seed_t *seed = ansi_new();
	seed->seed = bits_from_hex(str_is_hex(cstr));
	seed_hash(seed);
	seed_tsh(seed);
	curve->seed = seed;
	return 1;
}

static bits_t *seed_process(seed_t *seed, const bits_t *first) {
	pari_sp ltop = avma;

	bits_t *result = bits_copy(first);

	long is = itos(seed->ansi.s);
	GEN seedi = bits_to_i(seed->seed);
	GEN two_g = int2n(seed->seed->bitlen);

	for (long i = 1; i <= is; ++i) {
		pari_sp btop = avma;
		GEN inner = addis(seedi, i);
		inner = modii(inner, two_g);

		bits_t *to_hash = bits_from_i(inner);
		if (to_hash->bitlen < seed->seed->bitlen) {
			bits_lengthenz(to_hash, seed->seed->bitlen - to_hash->bitlen);
		}
		unsigned char hashout[20];
		bits_sha1(to_hash, hashout);
		bits_t *Wi = bits_from_raw(hashout, 160);
		bits_concatz(result, Wi, NULL);

		bits_free(&to_hash);
		bits_free(&Wi);
		avma = btop;
	}

	avma = ltop;
	return result;
}

static GENERATOR(ansi_gen_equation_fp) {
	pari_sp ltop = avma;
	bits_t *c0 = bits_from_raw(curve->seed->hash20, 160);
	bits_shortenz(c0, 160 - itos(curve->seed->ansi.h));

	bits_t *W0 = bits_copy(c0);
	SET_BIT(W0->bits, 0, 0);

	bits_t *W = seed_process(curve->seed, W0);

	long ti = itos(curve->seed->ansi.t);
	GEN r = gen_0;
	for (long i = 1; i <= ti; ++i) {
		GEN Wi = stoi(GET_BIT(W->bits, i - 1));
		r = addii(r, mulii(Wi, int2n(ti - i)));
	}
	curve->seed->ansi.r = r;

	GEN r_inv = Fp_invsafe(r, curve->field);
	GEN a;
	GEN b2;
	do {
		a = random_int(cfg->bits);
		b2 = mulii(powis(a, 3), r_inv);
	} while (!Fp_issquare(b2, curve->field));
	GEN b = Fp_sqrt(b2, curve->field);

	curve->a = a;
	curve->b = b;

	gerepileall(ltop, 3, &curve->seed->ansi.r, &curve->a, &curve->b);
	bits_free(&c0);
	bits_free(&W0);
	bits_free(&W);
	return 1;
}

static GENERATOR(ansi_gen_equation_f2m) {
	pari_sp ltop = avma;
	bits_t *b0 = bits_from_raw(curve->seed->hash20, 160);
	bits_shortenz(b0, 160 - itos(curve->seed->ansi.h));

	bits_t *b = seed_process(curve->seed, b0);
	GEN ib = bits_to_i(b);
	if (gequal0(ib)) {
		avma = ltop;
		return -3;
	}
	GEN a = random_int(cfg->bits);
	curve->a = field_ielement(curve->field, a);
	curve->b = field_ielement(curve->field, ib);

	gerepileall(ltop, 2, &curve->a, &curve->b);
	bits_free(&b0);
	bits_free(&b);
	return 1;
}

GENERATOR(ansi_gen_equation) {
	switch (cfg->field) {
		case FIELD_PRIME:
			return ansi_gen_equation_fp(curve, args, state);
		case FIELD_BINARY:
			return ansi_gen_equation_f2m(curve, args, state);
		default:
			pari_err_BUG("Field not prime or binary?");
			return INT_MIN; /* NOT REACHABLE */
	}
}
