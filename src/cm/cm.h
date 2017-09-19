/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file cm.h
 */
#ifndef ECGEN_CM_H
#define ECGEN_CM_H

#include "io/cli.h"
#include "misc/config.h"

/**
 *
 * @param cfg
 * @return
 */
int cm_do(config_t *cfg);

#endif  // ECGEN_CM_H
