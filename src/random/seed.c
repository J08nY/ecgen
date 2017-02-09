/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "seed.h"
#include "io/input.h"

seed_t *seed_new() {
	seed_t *seed = pari_malloc(sizeof(seed_t));
	if (!seed) {
		perror("Couldn't malloc.");
		exit(1);
	}
	memset(seed, 0, sizeof(seed_t));
	return seed;
}

void seed_free(seed_t **seed) {
	if (*seed) {
		pari_free(*seed);
		*seed = NULL;
	}
}

GEN seed_stoi(const char *cstr) {
	pari_sp ltop = avma;
	GEN seed = gen_0;

	size_t len = strlen(cstr);
	for (size_t i = 0; i < len; ++i) {
		pari_sp btop = avma;
		GEN s = stoi(cstr[i]);
		s = shifti(s, (len - i - 1) * 8);
		seed = addii(seed, s);
		if (gc_needed(btop, 1)) gerepileall(btop, 1, seed);
	}

	return gerepilecopy(ltop, seed);
}

int seed_random(curve_t *curve, config_t *config, ...) {
	curve->seed = seed_new();
	curve->seed->seed = random_int(160);
	return 1;
}

int seed_argument(curve_t *curve, config_t *config, ...) {
	curve->seed = seed_new();
	curve->seed->seed = seed_stoi(config->seed);
	return 1;
}

int seed_input(curve_t *curve, config_t *config, ...) {
	pari_sp ltop = avma;

	GEN str = fread_string(in, "seed:", '\n');
	const char *cstr = GSTR(str);
	if (strlen(cstr) < 20) {
		fprintf(stderr, "SEED must be at least 160 bits(20 characters).\n");
		avma = ltop;
		return 0;
	}

	GEN seed = seed_stoi(cstr);
	gerepileall(ltop, 1, &seed);

	curve->seed = seed_new();
	curve->seed->seed = seed;
	return 1;
}