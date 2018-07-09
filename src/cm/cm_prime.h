/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#ifndef ECGEN_CM_PRIME_H
#define ECGEN_CM_PRIME_H

#include "misc/config.h"
#include "misc/types.h"

typedef struct {
	/* Stuff filled with qdisc_next. */
	GEN p;
	GEN t;
	GEN D;

	/* Stuff for saving state. */
	GEN order;
	GEN r;
	GEN i;
	GEN* Sp;
	size_t nprimes;
} cm_prime_qdisc_t;

/**
 * Algorithm mostly from:
 *     Constructing elliptic curves of prime order
 *     by Reinier Broker and Peter Stevenhagen
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(cm_gen_curve_prime);

#endif  // ECGEN_CM_PRIME_H
