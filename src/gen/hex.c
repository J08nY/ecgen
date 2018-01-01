/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "hex.h"
#include "exhaustive/arg.h"
#include "field.h"
#include "util/bits.h"
#include "util/memory.h"

static char *hex_points(point_t *points[], size_t len) {
	char *p[len];
	for (size_t i = 0; i < len; ++i) {
		point_t *pt = points[i];
		GEN fx = field_elementi(gel(pt->point, 1));
		GEN fy = field_elementi(gel(pt->point, 2));
		p[i] = pari_sprintf("%P0#*x,%P0#*x,", cfg->hex_digits, fx,
		                    cfg->hex_digits, fy);
	}

	size_t total = 1;
	for (size_t i = 0; i < len; ++i) {
		total += strlen(p[i]);
	}

	char *result = try_calloc(total);
	for (size_t i = 0; i < len; ++i) {
		strcat(result, p[i]);
		pari_free(p[i]);
	}
	return result;
}

CHECK(hex_check_param) {
	HAS_ARG(args);

	char *search_hex = try_strdup(args->args);
	char *p = search_hex;
	for (; *p; ++p) *p = (char)tolower(*p);

	char *params[OFFSET_END] = {NULL};
	bool pari[OFFSET_END] = {false};

	if (state >= OFFSET_SEED) {
		if (curve->seed && curve->seed->seed) {
			params[OFFSET_SEED] = bits_to_hex(curve->seed->seed);
		}
	}

	if (state >= OFFSET_FIELD) {
		if (cfg->field == FIELD_PRIME) {
			params[OFFSET_FIELD] =
			    pari_sprintf("%P0#*x", cfg->hex_digits, curve->field);
			pari[OFFSET_FIELD] = true;
		} else if (cfg->field == FIELD_BINARY) {
		}
	}

	if (state >= OFFSET_A) {
		params[OFFSET_A] =
		    pari_sprintf("%P0#*x", cfg->hex_digits, field_elementi(curve->a));
		pari[OFFSET_A] = true;
	}

	if (state >= OFFSET_B) {
		params[OFFSET_B] =
		    pari_sprintf("%P0#*x", cfg->hex_digits, field_elementi(curve->b));
		pari[OFFSET_B] = true;
	}

	if (state >= OFFSET_ORDER) {
		params[OFFSET_ORDER] =
		    pari_sprintf("%P0#*x", cfg->hex_digits, curve->order);
		pari[OFFSET_ORDER] = true;
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
			if (pari[i]) {
				pari_free(params[i]);
			} else {
				try_free(params[i]);
			}
		}
	}
	try_free(search_hex);
	return result;
}