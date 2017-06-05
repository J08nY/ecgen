/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "memory.h"
#include <pari/pari.h>

void *alloc(void *(*fun)(size_t), size_t size) {
	void *result = fun(size);
	if (!result) {
		perror("Couldn't alloc.");
		exit(EXIT_FAILURE);
	}
	return result;
}

void *try_malloc(size_t size) { return alloc(pari_malloc, size); }

void *try_calloc(size_t size) { return alloc(pari_calloc, size); }

void *try_realloc(void *ptr, size_t size) {
	void *result = pari_realloc(ptr, size);
	if (!result) {
		perror("Couldn't alloc.");
		exit(EXIT_FAILURE);
	}
	return result;
}
