/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file str.h
 */
#ifndef ECGEN_STR_H
#define ECGEN_STR_H

#include <stddef.h>

/**
 * @brief
 * @param hex_str
 * @return
 */
const char *str_is_hex(const char *hex_str);

/**
 * @brief
 * @param strings
 * @param len
 * @return
 */
char *str_join(char *strings[], size_t len);

#endif  // ECGEN_STR_H
