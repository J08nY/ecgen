/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file curve.h
 */
#ifndef ECGEN_GEN_CURVE_H
#define ECGEN_GEN_CURVE_H

#include <pari/pari.h>
#include "misc/types.h"

/**
 * GENERATOR(gen_f)
 * Creates a curve GEN in curve_t curve from field, a and b.
 * Always succeeds.
 *
 * @param curve A curve_t being generated
 * @param args unused
 * @param state
 * @return state diff
 */
GENERATOR(curve_gen_any);

/**
 * GENERATOR(gen_f)
 *
 * @param curve
 * @param args
 * @param state
 * @return state diff
 */
GENERATOR(curve_gen_any_twist);

/**
 * CHECK(check_f)
 * Checks that the curve has non-zero discriminant.
 *
 * @param curve
 * @param args
 * @return
 */
CHECK(curve_check_nonzero);

/**
 *
 * @param curve
 * @param from
 * @param to
 * @return
 */
UNROLL(curve_unroll);

#endif  // ECGEN_GEN_CURVE_H
