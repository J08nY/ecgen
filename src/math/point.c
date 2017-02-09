/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "point.h"

point_t *gerepile_point(pari_sp ltop, point_t *point) {
	if (point) {
		gerepileall(ltop, 2, &point->point, &point->order);
	}
	return point;
}