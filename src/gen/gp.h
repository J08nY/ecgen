/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file gp.h
 */
#ifndef ECGEN_GP_H
#define ECGEN_GP_H

#include "misc/types.h"

/**
 * @brief
 * @param curve
 * @param args
 * @return
 */
GENERATOR(gp_gen);

/**
 * @brief
 * @param curve
 * @param args
 * @param state
 * @return
 */
CHECK(gp_check);

#endif  // ECGEN_GP_H
