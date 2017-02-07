/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_GENERATORS_H
#define ECGEN_GENERATORS_H

#include "types.h"

enum gen_offset {
	OFFSET_SEED,
	OFFSET_FIELD,
	OFFSET_A,
	OFFSET_B,
	OFFSET_CURVE,
	OFFSET_POINTS
};

/**
 *
 * @param generators
 */
void gen_init(gen_t generators[], config_t *config);

#endif  // ECGEN_GENERATORS_H
