/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "hex.h"
#include "exhaustive/arg.h"
#include "field.h"
#include "util/bits.h"
#include "util/memory.h"

static char *hex_points(point_t *points[], size_t len) {
	char *x[len];
	char *y[len];
	for (size_t i = 0; i < len; ++i) {
		point_t *pt = points[i];
		bits_t *x_bits = bits_from_i(field_elementi(gel(pt->point, 1)));
		bits_t *y_bits = bits_from_i(field_elementi(gel(pt->point, 2)));
		x[i] = bits_to_hex(x_bits);
		y[i] = bits_to_hex(y_bits);
		bits_free(&x_bits);
		bits_free(&y_bits);
	}

	size_t total = 0;
	for (size_t i = 0; i < len; ++i) {
		total += strlen(x[i]) + strlen(y[i]);
	}

	char *result = try_calloc(total);
	for (size_t i = 0; i < len; ++i) {
		strcat(result, x[i]);
		strcat(result, y[i]);
		try_free(x[i]);
		try_free(y[i]);
	}
	return result;
}

CHECK(hex_check_param) {
	HAS_ARG(args);

	char *search_hex = try_strdup(args->args);
	char *p = search_hex;
	for (; *p; ++p) *p = (char)tolower(*p);

	char *params[OFFSET_END] = {NULL};

	if (state >= OFFSET_SEED) {
		params[OFFSET_SEED] = bits_to_hex(curve->seed->seed);
	}

	if (state >= OFFSET_FIELD) {
		if (cfg->field == FIELD_PRIME) {
			bits_t *temp = bits_from_i(curve->field);
			params[OFFSET_FIELD] = bits_to_hex(temp);
			bits_free(&temp);
		} else if (cfg->field == FIELD_BINARY) {
		}
	}

	if (state >= OFFSET_A) {
		bits_t *temp = bits_from_i(field_elementi(curve->a));
		params[OFFSET_A] = bits_to_hex(temp);
		bits_free(&temp);
	}

	if (state >= OFFSET_B) {
		bits_t *temp = bits_from_i(field_elementi(curve->b));
		params[OFFSET_B] = bits_to_hex(temp);
		bits_free(&temp);
	}

	if (state >= OFFSET_ORDER) {
		bits_t *temp = bits_from_i(curve->order);
		params[OFFSET_ORDER] = bits_to_hex(temp);
		bits_free(&temp);
	}

	if (state >= OFFSET_GENERATORS) {
		params[OFFSET_GENERATORS] = hex_points(curve->generators, curve->ngens);
	}

	if (state >= OFFSET_POINTS) {
		params[OFFSET_POINTS] = hex_points(curve->points, curve->npoints);
	}

	int result = OFFSET_FIELD - state;
	for (offset_e i = OFFSET_SEED; i < OFFSET_END; ++i) {
		if (params[i]) {
			if (strstr(params[i], search_hex)) {
				result = 1;
				break;
			}
		}
	}
	try_free(search_hex);
	return result;
}