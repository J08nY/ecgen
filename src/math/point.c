/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "point.h"

point_t *point_new() {}

point_t **points_new(size_t num) {}

void point_free(point_t **point) {}

void points_free(point_t ***point) {}

int point_random(curve_t *curve, config_t *config, ...) {}

int points_random(curve_t *curve, config_t *config, ...) {
	va_list arg;
	va_start(arg, config);
	size_t npoints = va_arg(arg, size_t);
	va_end(arg);

	curve->points = points_new(npoints);
	curve->npoints = npoints;
	for (size_t i = 0; i < npoints; ++i) {
	}
}

int points_prime(curve_t *curve, config_t *config, ...) {}

int points_generators(curve_t *curve, config_t *config, ...) {}