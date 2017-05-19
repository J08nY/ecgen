/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include <stddef.h>

#ifndef ECGEN_MEMORY_H
#define ECGEN_MEMORY_H

/**
 * @brief
 * @param size
 * @return
 */
void *try_malloc(size_t size);

/**
 * @brief
 * @param size
 * @return
 */
void *try_calloc(size_t size);

/**
 * @brief
 * @param ptr
 * @param size
 * @return
 */
void *try_realloc(void *ptr, size_t size);

#endif  // ECGEN_MEMORY_H
