/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_POINT_H
#define ECGEN_POINT_H

#include <pari/pari.h>
#include "types.h"

point_t *gerepile_point(pari_sp ltop, point_t *point);

#endif  // ECGEN_POINT_H
