
#ifndef ECGEN_TEST_MEMORY_H
#define ECGEN_TEST_MEMORY_H

#include <stddef.h>

char *cr_strdup(const char *str);

void *cr_memdup(const void *str, size_t len);

void *cr_simple_calloc(size_t size);

#endif //ECGEN_TEST_MEMORY_H
