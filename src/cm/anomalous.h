/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file anomalous.h
 */
#ifndef ECGEN_CM_ANOMALOUS_H
#define ECGEN_CM_ANOMALOUS_H

#include <pari/pari.h>
#include "exhaustive/arg.h"
#include "io/cli.h"
#include "misc/types.h"

typedef struct {
	GEN d;
	GEN j;
	GEN alpha;
} disc_t;

/**
 * GENERATOR(gen_f)
 *
 * @param curve A curve_t being generated
 * @param args the index of the discriminant to use, in the disc_table
 * @return state diff
 */
GENERATOR(anomalous_gen_field);

/**
 * GENERATOR(gen_f)
 *
 * @param curve A curve_t being generated
 * @param args the index of the discriminant to use, in the disc_table
 * @return state diff
 */
GENERATOR(anomalous_gen_equation);

/**
 * GENERATOR(gen_f)
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @return state diff
 */
GENERATOR(anomalous_gen_order);

/**
 * @brief Initialize anomalous generation, allocate and set the disc_table.
 */
void anomalous_init();

/**
 * @brief Deinitialize anomalous generation, free the discriminants from the
 * disc_table.
 */
void anomalous_quit();

#endif  // ECGEN_CM_ANOMALOUS_H
