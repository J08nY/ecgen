/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file str.h
 */
#ifndef ECGEN_STR_H
#define ECGEN_STR_H

#include <stddef.h>

/**
 * @brief
 * @param strings
 * @param len
 * @return
 */
char *str_join(char *strings[], size_t len);

#endif  // ECGEN_STR_H
