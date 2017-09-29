/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file arg.h
 */
#ifndef ECGEN_ARG_H
#define ECGEN_ARG_H

#include "misc/types.h"

#define HAS_ARG(args)                                                 \
	if (!(args)) {                                                    \
		fprintf(err, "No args to an arged function. %s\n", __func__); \
		return INT_MIN;                                               \
	}

/**
 * @brief
 * @return
 */
arg_t *arg_new(void);

/**
 * @brief
 * @param arg
 */
void arg_free(arg_t **arg);

#endif  // ECGEN_ARG_H
