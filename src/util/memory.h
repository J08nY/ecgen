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
