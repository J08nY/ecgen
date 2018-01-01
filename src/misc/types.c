/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "types.h"

GENERATOR(gen_skip) { return 1; }

CHECK(check_skip) { return 1; }

UNROLL(unroll_skip) { return -1; }
