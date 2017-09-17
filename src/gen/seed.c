/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include "seed.h"
#include <io/config.h>
#include "io/output.h"
#include "types.h"
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

