/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file exhaustive.h
 */
#ifndef ECGEN_GENERATORS_H
#define ECGEN_GENERATORS_H

#include "misc/types.h"

/**
 *
 * @param unrolls
 * @param cfg
 */
void exhaustive_uinit(unroll_t *unrolls, const config_t *cfg);

/**
 *
 * @param curve
 * @param cfg
 * @param generators
 * @param argss
 * @param unrolls
 * @param start_offset
 * @param end_offset
 * @param retry
 * @return
 */
int exhaustive_gen_retry(curve_t *curve, const config_t *cfg,
                         gen_t generators[], arg_t *argss[], unroll_t unrolls[],
                         offset_e start_offset, offset_e end_offset, int retry);

/**
 *
 * @param curve
 * @param config
 * @param generators
 * @param argss
 * @param unrolls
 * @param start_offset
 * @param end_offset
 * @return
 */
int exhaustive_gen(curve_t *curve, const config_t *cfg, gen_t generators[],
                   arg_t *argss[], unroll_t unrolls[], offset_e start_offset,
                   offset_e end_offset);

/**
 *
 * @param cfg
 * @return
 */
int exhaustive_do(config_t *cfg);

#endif  // ECGEN_GENERATORS_H
