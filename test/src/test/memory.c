
#include <string.h>
#include <criterion/alloc.h>
#include "memory.h"

char *cr_strdup(const char *str) {
	size_t len = strlen(str);
	return cr_memdup(str, len + 1);
}

void *cr_memdup(const void *str, size_t len) {
	void *result = cr_malloc(len);
	memcpy(result, str, len);
	return result;
}

void *cr_simple_calloc(size_t size) { return cr_calloc(1, size); }