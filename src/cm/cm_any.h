/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#ifndef ECGEN_CM_ANY_H
#define ECGEN_CM_ANY_H

#include "misc/types.h"

typedef struct {
	GEN p;
	GEN d;
} cm_any_qdisc_t;

/**
 * @brief
 * @param order
 * @return
 */
curve_t *cm_any_curve(GEN order);

#endif  // ECGEN_CM_ANY_H
