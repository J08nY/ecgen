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
	GEN d;
	GEN j;
	GEN alpha;
} disc_t;

/**
 * GENERATOR(gen_t)
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args the index of the discriminant to use, in the disc_table
 * @return state diff
 */
int anomalous_field(curve_t *curve, const config_t *cfg, arg_t *args);

/**
 * GENERATOR(gen_t)
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args the index of the discriminant to use, in the disc_table
 * @return state diff
 */
int anomalous_equation(curve_t *curve, const config_t *cfg, arg_t *args);

/**
 * GENERATOR(gen_t)
 *
 * @param curve A curve_t being generated
 * @param cfg An application config
 * @param args unused
 * @return state diff
 */
int anomalous_order(curve_t *curve, const config_t *cfg, arg_t *args);

/**
 * @brief Initialize anomalous generation, allocate and set the disc_table.
 */
void anomalous_init();

/**
 * @brief Deinitialize anomalous generation, free the discriminants from the
 * disc_table.
 */
void anomalous_quit();

#endif  // ECGEN_ANOMALOUS_H
