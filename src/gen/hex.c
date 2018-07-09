/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "hex.h"
#include "exhaustive/arg.h"
#include "field.h"
#include "util/bits.h"
#include "util/memory.h"
#include "util/str.h"

static char *hex_point(point_t *point) {
	GEN fx = field_elementi(gel(point->point, 1));
	GEN fy = field_elementi(gel(point->point, 2));
	char *fxs = pari_sprintf("%P0#*x", cfg->hex_digits, fx);
	char *fxy = pari_sprintf("%P0#*x", cfg->hex_digits, fy);
	char *result = str_joinv(",", fxs, fxy, NULL);
	pari_free(fxs);
	pari_free(fxy);
	return result;
}

static char *hex_points(point_t *points[], size_t len) {
	char *p[len];
	for (size_t i = 0; i < len; ++i) {
		point_t *pt = points[i];
		p[i] = hex_point(pt);
	}

	size_t total = 1;
	for (size_t i = 0; i < len; ++i) {
		total += strlen(p[i]);
	}

	char *result = try_calloc(total);
	for (size_t i = 0; i < len; ++i) {
		strcat(result, p[i]);
		try_free(p[i]);
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
		char *subgroups[curve->ngens];
		for (size_t i = 0; i < curve->ngens; ++i) {
			subgroups[i] = hex_point(curve->generators[i]->generator);
		}
		params[OFFSET_GENERATORS] = str_join(",", subgroups, curve->ngens);
		for (size_t i = 0; i < curve->ngens; ++i) {
			try_free(subgroups[i]);
		}
	}

	if (state >= OFFSET_POINTS) {
		char *subgroups[curve->ngens];
		for (size_t i = 0; i < curve->ngens; ++i) {
			subgroups[i] = hex_points(curve->generators[i]->points,
			                          curve->generators[i]->npoints);
		}
		params[OFFSET_POINTS] = str_join(",", subgroups, curve->ngens);
		for (size_t i = 0; i < curve->ngens; ++i) {
			try_free(subgroups[i]);
		}
	}

	int result = OFFSET_FIELD - state;
	for (offset_e i = OFFSET_SEED; i < OFFSET_END; ++i) {
		if (params[i]) {
			if (result != 1 && strstr(params[i], search_hex)) {
				result = 1;
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