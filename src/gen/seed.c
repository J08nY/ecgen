/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include "seed.h"
#include <io/config.h>
#include "io/output.h"
#include "util/memory.h"

seed_t *seed_new(void) { return try_calloc(sizeof(seed_t)); }

seed_t *seed_copy(const seed_t *src, seed_t *dest) {
	if (src->seed) dest->seed = gcopy(src->seed);
	return dest;
}

seed_t *seed_new_copy(const seed_t *src) {
	seed_t *result = seed_new();
	return seed_copy(src, result);
}

seed_t *seed_clone(const seed_t *src, seed_t *dest) {
	if (src->seed) dest->seed = gclone(src->seed);
	return dest;
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
		seed_str[len + 2] = 0;
	} else {
		seed_str = try_malloc(len + 1);
		strncpy(seed_str, cstr, len);
	}
	GEN i = strtoi(seed_str);

	return gerepilecopy(ltop, i);
}

static char *seed_itos(GEN seed) {
	pari_sp ltop = avma;
	char *result = pari_sprintf("%Px", seed);

	size_t seed_len = strlen(result);
	char *seed_str = try_malloc(seed_len + 1);
	strcpy(seed_str, result);

	avma = ltop;
	return seed_str;
}

GENERATOR(seed_gen_random) {
	curve->seed = seed_new();
	curve->seed->seed = random_int(160);
	curve->seed->raw = seed_itos(curve->seed->seed);
	curve->seed->raw_len = strlen(curve->seed->raw);
	return 1;
}

GENERATOR(seed_gen_argument) {
	curve->seed = seed_new();
	curve->seed->seed = seed_stoi(cfg->seed);

	size_t seed_len = strlen(cfg->seed);
	char *seed = try_malloc(seed_len + 1);
	strcpy(seed, cfg->seed);

	curve->seed->raw = seed;
	curve->seed->raw_len = seed_len;
	return 1;
}

GENERATOR(seed_gen_input) {
	pari_sp ltop = avma;

	GEN str = input_string("seed:");
	const char *cstr = GSTR(str);
	if (strlen(cstr) < 20) {
		fprintf(err, "SEED must be at least 160 bits(20 characters).\n");
		avma = ltop;
		return 0;
	}
	GEN seed = seed_stoi(cstr);

	curve->seed = seed_new();
	curve->seed->seed = gerepilecopy(ltop, seed);

	size_t seed_len = strlen(cstr);
	char *seed_str = try_malloc(seed_len + 1);
	strcpy(seed_str, cfg->seed);

	curve->seed->raw = seed_str;
	curve->seed->raw_len = seed_len;
	return 1;
}
