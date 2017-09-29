/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_CUSTOM_H
#define ECGEN_CUSTOM_H

#include "misc/types.h"
#include "misc/config.h"

typedef struct {
	GEN p;
	GEN t;
	GEN v;
	GEN D;
} custom_quadr_t;

/**
 * @brief
 * @return
 */
curve_t * custom_curve();

#endif //ECGEN_CUSTOM_H
