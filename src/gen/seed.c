/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include "seed.h"
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
	GEN seed = gen_0;

	size_t len = strlen(cstr);
	for (size_t i = 0; i < len; ++i) {
		pari_sp btop = avma;
		GEN s = stoi(cstr[i]);
		s = shifti(s, (len - i - 1) * 8);
		seed = addii(seed, s);
		gerepileall(btop, 1, &seed);
	}

	return gerepilecopy(ltop, seed);
}

static char *seed_itos(GEN seed) {
	pari_sp ltop = avma;
	GEN bits = binary_zv(seed);

	long len = glength(bits);
	long bytes = (len / 8) + (len % 8 == 0 ? 0 : 1);
	char *result = try_malloc((size_t)bytes);

	for (long i = 0; i < len; ++i) {
		// TODO
	}
	avma = ltop;
	return result;
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
	curve->seed->raw = cfg->seed;
	curve->seed->raw_len = strlen(cfg->seed);
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
	return 1;
}
