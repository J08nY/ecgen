/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_CUSTOM_H
#define ECGEN_CUSTOM_H

#include "misc/config.h"
#include "misc/types.h"

typedef struct {
	/* Stuff filled with custom_quadr_next. */
	GEN p;
	GEN t;
	GEN D;

	/* Stuff for saving state. */
	GEN order;
	GEN r;
	GEN i;
	GEN* Sp;
	size_t nprimes;
} custom_quadr_t;

/**
 * Algorithm mostly from:
 *     Constructing elliptic curves of prime order
 *     by Reinier Broker and Peter Stevenhagen
 * @return
 */
curve_t* custom_curve();

#endif  // ECGEN_CUSTOM_H
