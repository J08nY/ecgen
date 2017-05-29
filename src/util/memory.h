/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file memory.h
 */
#ifndef ECGEN_MEMORY_H
#define ECGEN_MEMORY_H

#include <stddef.h>

/**
 * @brief Try mallocing some memory, exit(EXIT_FAILURE) if it fails.
 * @param size
 * @return
 */
void *try_malloc(size_t size);

/**
 * @brief Try callocing some memory, exit(EXIT_FAILURE) if it fails.
 * @param size
 * @return
 */
void *try_calloc(size_t size);

/**
 * @brief Try reallocing some memory, exit(EXIT_FAILURE) if it fails.
 * @param ptr
 * @param size
 * @return
 */
void *try_realloc(void *ptr, size_t size);

#endif  // ECGEN_MEMORY_H
