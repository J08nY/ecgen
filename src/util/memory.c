/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "memory.h"
#include <pari/pari.h>

#define ECGEN_PARI_MEM
#ifdef ECGEN_PARI_MEM

static void *(*malloc_func)(size_t) = pari_malloc;

static void *(*calloc_func)(size_t) = pari_calloc;

static void *(*realloc_func)(void *, size_t) = pari_realloc;

static void (*free_func)(void *) = pari_free;

#else

static void *(*malloc_func)(size_t) = malloc;

void *calloc_wrapper(size_t n) { return calloc(1, n); }

static void *(*calloc_func)(size_t) = calloc_wrapper;

static void *(*realloc_func)(void *, size_t) = realloc;

static void (*free_func)(void *) = free;

#endif

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

char *try_strdup(const char *str) {
	size_t len = strlen(str);
	return try_memdup(str, len + 1);
}

void *try_memdup(const void *mem, size_t len) {
	void *result = try_malloc(len);
	memcpy(result, mem, len);
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