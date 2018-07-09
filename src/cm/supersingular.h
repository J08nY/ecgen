/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#ifndef ECGEN_CM_SUPERSINGULAR_H
#define ECGEN_CM_SUPERSINGULAR_H

#include "misc/types.h"

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(supersingular_gen_equation);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
GENERATOR(supersingular_gen_order);

#endif  // ECGEN_CM_SUPERSINGULAR_H
