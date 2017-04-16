/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file anomalous.h
 */
#ifndef ECGEN_ANOMALOUS_H
#define ECGEN_ANOMALOUS_H

#include <pari/pari.h>
#include "io/cli.h"
#include "math/arg.h"
#include "math/types.h"

typedef struct disc_t {
	GEN D;
	GEN j;
	GEN a;
} disc_t;

int anomalous_field(curve_t *curve, const config_t *cfg, arg_t *args);

int anomalous_equation(curve_t *curve, const config_t *cfg, arg_t *args);

int anomalous_order(curve_t *curve, const config_t *cfg, arg_t *args);

void anomalous_init();

void anomalous_quit();

#endif  // ECGEN_ANOMALOUS_H
