/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include "seed.h"
#include <sha1/sha1.h>
#include "io/output.h"
#include "util/binascii.h"
#include "util/memory.h"

seed_t *seed_new(void) { return try_calloc(sizeof(seed_t)); }

static seed_t *seed_cpy(const seed_t *src, seed_t *dest) {
	if (src->hex) {
		dest->hex = try_strdup(src->hex);
		dest->hex_len = src->hex_len;
	}
	if (src->raw) {
		dest->raw = try_memdup(src->raw, src->raw_len);
		dest->raw_len = src->raw_len;
	}
	if (src->hash20) {
		dest->hash20 = try_memdup(src->hash20, 20);
	}
	if (src->W) {
		dest->W = try_memdup(src->W, src->W_len);
		dest->W_len = src->W_len;
	}
	return dest;
}

seed_t *seed_copy(const seed_t *src, seed_t *dest) {
	if (src->seed) dest->seed = gcopy(src->seed);
	return seed_cpy(src, dest);
}

seed_t *seed_new_copy(const seed_t *src) {
	seed_t *result = seed_new();
	return seed_copy(src, result);
}

seed_t *seed_clone(const seed_t *src, seed_t *dest) {
	if (src->seed) dest->seed = gclone(src->seed);
	return seed_cpy(src, dest);
}

seed_t *seed_new_clone(const seed_t *src) {
	seed_t *result = seed_new();
	return seed_clone(src, result);
}

void seed_free(seed_t **seed) {
	if (*seed) {
		if ((*seed)->seed && isclone((*seed)->seed)) {
			gunclone((*seed)->seed);
		}
		if ((*seed)->hex) {
			try_free((*seed)->hex);
		}
		if ((*seed)->raw) {
			try_free((*seed)->raw);
		}
		if ((*seed)->hash20) {
			try_free((*seed)->hash20);
		}
		try_free(*seed);
		*seed = NULL;
	}
}

static GEN seed_stoi(const char *cstr) {
	pari_sp ltop = avma;

	size_t len = strlen(cstr);
	char *seed_str;
	if (len <= 3 || !(cstr[0] == '0' && (cstr[1] == 'x' || cstr[1] == 'X'))) {
		seed_str = try_malloc((size_t)(len + 3));
		strncpy(seed_str + 2, cstr, len);
		seed_str[0] = '0';
		seed_str[1] = 'x';
	} else {
		seed_str = try_malloc(len + 1);
		strncpy(seed_str, cstr, len);
	}
	GEN i = strtoi(seed_str);

	try_free(seed_str);
	return gerepilecopy(ltop, i);
}

static char *seed_itos(GEN seed) {
	pari_sp ltop = avma;
	char *result = pari_sprintf("%Px", seed);
	char *seed_str = try_strdup(result);

	avma = ltop;
	return seed_str;
}

static char *seed_strip(const char *cstr) {
	char *seed_str = try_malloc(strlen(cstr) + 1);
	char *prefix = strstr(cstr, "0x");
	if (prefix != NULL) {
		strcpy(seed_str, cstr + 2);
	} else {
		strcpy(seed_str, cstr);
	}
	return seed_str;
}

static void seed_raw(seed_t *seed) {
	seed->raw = binascii_itob(seed->seed, ENDIAN_BIG);
	seed->raw_len = binascii_blen(seed->seed);
}

static void seed_hash(seed_t *seed) {
	seed->hash20 = try_malloc(20);
	SHA_CTX ctx = {};
	SHA1_Init(&ctx);
	SHA1_Update(&ctx, seed->raw, (int)seed->raw_len);
	SHA1_Final(seed->hash20, &ctx);
}

static void seed_W(seed_t *seed, const config_t *cfg) {
	GEN t = utoi(cfg->bits - 1);
	GEN s = floorr(rdivii(subis(t, 1), stoi(160), DEFAULTPREC));
	GEN h = subis(t, 160);
}

GENERATOR(seed_gen_random) {
	seed_t *seed = seed_new();
	seed->seed = random_int(160);
	seed->hex = seed_itos(seed->seed);
	seed->hex_len = strlen(seed->hex);
	seed_raw(seed);
	seed_hash(seed);
	seed_W(seed, cfg);
	curve->seed = seed;
	return 1;
}

GENERATOR(seed_gen_argument) {
	seed_t *seed = seed_new();
	seed->seed = seed_stoi(cfg->seed);
	seed->hex = seed_strip(cfg->seed);
	seed->hex_len = strlen(seed->hex);
	seed_raw(seed);
	seed_hash(seed);
	seed_W(seed, cfg);
	curve->seed = seed;
	return 1;
}

GENERATOR(seed_gen_input) {
	pari_sp ltop = avma;

	GEN str = input_string("seed:");
	const char *cstr = GSTR(str);
	if (strlen(cstr) < 40) {
		fprintf(err, "SEED must be at least 160 bits(40 hex characters).\n");
		avma = ltop;
		return 0;
	}

	seed_t *seed = seed_new();
	seed->seed = seed_stoi(cstr);
	seed->hex = seed_strip(cstr);
	seed->hex_len = strlen(seed->hex);
	seed_raw(seed);
	seed_hash(seed);
	seed_W(seed, cfg);
	curve->seed = seed;
	return 1;
}
