/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file arg.h
 */
#ifndef ECGEN_ARG_H
#define ECGEN_ARG_H

#include "types.h"

/**
 * @brief
 * @return
 */
arg_t *arg_new();

/**
 * @brief
 * @param arg
 */
void arg_free(arg_t **arg);

#endif  // ECGEN_ARG_H
