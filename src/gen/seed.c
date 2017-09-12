/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include "seed.h"
#include "io/output.h"
#include "util/bits.h"
#include "util/memory.h"

seed_t *seed_new(void) { return try_calloc(sizeof(seed_t)); }

seed_t *seed_copy(const seed_t *src, seed_t *dest) {
	if (src->seed) {
		dest->seed = bits_copy(src->seed);
	}
	if (src->hash20) {
		dest->hash20 = try_memdup(src->hash20, 20);
	}
	if (src->W) {
		dest->W = bits_copy(src->W);
	}
	return dest;
}

seed_t *seed_new_copy(const seed_t *src) {
	seed_t *result = seed_new();
	return seed_copy(src, result);
}

seed_t *seed_clone(const seed_t *src, seed_t *dest) {
	return seed_copy(src, dest);
}

seed_t *seed_new_clone(const seed_t *src) {
	seed_t *result = seed_new();
	return seed_clone(src, result);
}

void seed_free(seed_t **seed) {
	if (*seed) {
		if ((*seed)->seed) {
			bits_free(&(*seed)->seed);
		}
		if ((*seed)->hash20) {
			try_free((*seed)->hash20);
		}
		if ((*seed)->W) {
			bits_free(&(*seed)->W);
		}
		try_free(*seed);
		*seed = NULL;
	}
}

bool seed_valid(const char *hex_str) {
	size_t len = strlen(hex_str);
	if (len < 40) {
		return false;
	}
	const char *str_start = hex_str;
	if (hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X')) {
		str_start = hex_str + 2;
	}
	while (*str_start != 0) {
		char c = *str_start++;
		if (!isxdigit(c)) return false;
	}
	return true;
}

static bits_t *seed_stoi(const char *cstr) {
	const char *seed_str = cstr;
	const char *prefix = strstr(cstr, "0x");
	if (prefix != NULL) seed_str = prefix + 2;
	return bits_from_hex(seed_str);
}

static void seed_hash(seed_t *seed) {
	seed->hash20 = try_malloc(20);
	bits_sha1(seed->seed, seed->hash20);
}

static void seed_W(seed_t *seed, const config_t *cfg) {
	GEN t = utoi(cfg->bits);
	GEN s = floorr(rdivii(subis(t, 1), stoi(160), DEFAULTPREC));
	GEN h = subii(t, mulis(s, 160));
}

GENERATOR(seed_gen_random) {
	seed_t *seed = seed_new();
	seed->seed = bits_from_i(random_int(160));
	seed_hash(seed);
	seed_W(seed, cfg);
	curve->seed = seed;
	return 1;
}

GENERATOR(seed_gen_argument) {
	seed_t *seed = seed_new();
	seed->seed = seed_stoi(cfg->seed);
	seed_hash(seed);
	seed_W(seed, cfg);
	curve->seed = seed;
	return 1;
}

GENERATOR(seed_gen_input) {
	pari_sp ltop = avma;

	GEN str = input_string("seed:");
	const char *cstr = GSTR(str);
	if (!seed_valid(cstr)) {
		fprintf(err, "SEED must be at least 160 bits(40 hex characters).\n");
		avma = ltop;
		return 0;
	}

	seed_t *seed = seed_new();
	seed->seed = seed_stoi(cstr);
	seed_hash(seed);
	seed_W(seed, cfg);
	curve->seed = seed;
	return 1;
}
