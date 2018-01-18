/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */

#include "seed.h"
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
		bits_free(&(*seed)->seed);
		if ((*seed)->hash20) {
			try_free((*seed)->hash20);
		}
		switch ((*seed)->type) {
			case SEED_ANSI:
				break;
			case SEED_BRAINPOOL:
			case SEED_BRAINPOOL_RFC:
				bits_free(&(*seed)->brainpool.seed_a);
				bits_free(&(*seed)->brainpool.seed_b);
				break;
			case SEED_FIPS:
				break;
			case SEED_NONE:
				break;
		}
		try_free(*seed);
		*seed = NULL;
	}
}

UNROLL(seed_unroll) {
	seed_free(&curve->seed);
	return -1;
}
