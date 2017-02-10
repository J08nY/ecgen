/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_ORDER_H
#define ECGEN_ORDER_H

#include "types.h"

/**
 *
 * @param curve
 * @param cfg
 * @param ...
 * @return
 */
int order_init(curve_t *curve, config_t *cfg, ...);

/**
 *
 * @param curve
 * @param cfg
 * @param ...
 * @return
 */
int order_prime(curve_t *curve, config_t *cfg, ...);

#endif //ECGEN_ORDER_H
