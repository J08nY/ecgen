/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "memory.h"
#include <pari/pari.h>

static void *(*malloc_func)(size_t) = pari_malloc;

static void *(*calloc_func)(size_t) = pari_calloc;

static void *(*realloc_func)(void *, size_t) = pari_realloc;

static void (*free_func)(void *) = pari_free;

void *alloc(void *(*fun)(size_t), size_t size) {
	void *result = fun(size);
	if (!result) {
		perror("Couldn't alloc.");
		exit(EXIT_FAILURE);
	}
	return result;
}

void *try_malloc(size_t size) { return alloc(malloc_func, size); }

void *try_calloc(size_t size) { return alloc(calloc_func, size); }

void *try_realloc(void *ptr, size_t size) {
	void *result = realloc_func(ptr, size);
	if (!result) {
		perror("Couldn't alloc.");
		exit(EXIT_FAILURE);
	}
	return result;
}

void try_free(void *ptr) { free_func(ptr); }

void set_mem_funcs(void *(*malloc_fun)(size_t), void *(*calloc_fun)(size_t),
                   void *(*realloc_fun)(void *, size_t),
                   void (*free_fun)(void *)) {
	malloc_func = malloc_fun;
	calloc_func = calloc_fun;
	realloc_func = realloc_fun;
	free_func = free_fun;
}