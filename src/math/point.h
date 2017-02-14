/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_POINT_H
#define ECGEN_POINT_H

#include <pari/pari.h>
#include "types.h"
/**
 *
 * @return
 */
point_t *point_new();

/**
 *
 * @param num
 * @return
 */
point_t **points_new(size_t num);

/**
 *
 * @param point
 */
void point_free(point_t **point);

/**
 *
 * @param point
 */
void points_free(point_t ***point);

/**
 *
 * @param curve
 * @param config
 * @param ...
 * @return
 */
int point_random(curve_t *curve, config_t *config, ...);

/**
 *
 * @param curve
 * @param config
 * @param ...
 * @return
 */
int points_random(curve_t *curve, config_t *config, ...);

#endif  // ECGEN_POINT_H
