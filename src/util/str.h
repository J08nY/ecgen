/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file str.h
 */
#ifndef ECGEN_UTIL_STR_H
#define ECGEN_UTIL_STR_H

#include <stddef.h>

/**
 * @brief
 * @param hex_str
 * @return
 */
const char *str_is_hex(const char *hex_str);

/**
 * @brief
 * @param separator
 * @param strings
 * @param len
 * @return
 */
char *str_join(char *separator, char **strings, size_t len);

/**
 * @brief
 * @param separator
 * @param ...
 * @return
 */
char *str_joinv(char *separator, ...);

/**
 * @brief
 * @param strings
 * @param len
 * @return
 */
char *str_concat(char **strings, size_t len);

/**
 * @brief
 * @param str
 * @param c
 * @return
 */
size_t str_cnt(const char *str, const char c);

#endif  // ECGEN_UTIL_STR_H
