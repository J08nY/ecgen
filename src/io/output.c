/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include "output.h"
#include <parson/parson.h>
#include "gen/field.h"
#include "util/memory.h"

FILE *out;
FILE *err;
FILE *verbose;

char *output_malloc(const char *what) {
	char *s = try_calloc(sizeof(char) * (strlen(what) + 1));
	strcpy(s, what);
	return s;
}

char *output_scsv(curve_t *curve, const config_t *cfg) {
	pari_sp ltop = avma;
	char *params[OFFSET_END];

	for (int i = 0; i < OFFSET_END; ++i) {
		params[i] = NULL;
	}

	switch (cfg->field) {
		case FIELD_PRIME:
			params[OFFSET_FIELD] =
			    pari_sprintf("%P0#*x", cfg->hex_digits, curve->field);
			break;
		case FIELD_BINARY: {
			GEN field = field_params(curve->field);
			params[OFFSET_FIELD] =
			    pari_sprintf("%P#x,%P#x,%P#x,%P#x", gel(field, 1),
			                 gel(field, 2), gel(field, 3), gel(field, 4));
			break;
		}
	}

	if (curve->a)
		params[OFFSET_A] =
		    pari_sprintf("%P0#*x", cfg->hex_digits, field_elementi(curve->a));
	if (curve->b)
		params[OFFSET_B] =
		    pari_sprintf("%P0#*x", cfg->hex_digits, field_elementi(curve->b));

	if (curve->generators) {
		char *gens[curve->ngens];
		size_t len = 0;
		for (size_t i = 0; i < curve->ngens; ++i) {
			point_t *generator = curve->generators[i];
			GEN x = field_elementi(gel(generator->point, 1));
			GEN y = field_elementi(gel(generator->point, 2));
			gens[i] = pari_sprintf("%P0#*x,%P0#*x,%P#x,%P#x", cfg->hex_digits,
			                       x, cfg->hex_digits, y, generator->order,
			                       generator->cofactor);
			len += strlen(gens[i]);
		}
		size_t lenn = sizeof(char) * (len + curve->ngens);
		params[OFFSET_GENERATORS] = pari_calloc(lenn);
		for (size_t i = 0; i < curve->ngens; ++i) {
			if (i > 0) strncat(params[OFFSET_GENERATORS], ",", lenn - 1);
			strncat(params[OFFSET_GENERATORS], gens[i], lenn - 1);
			pari_free(gens[i]);
		}
	}

	if (curve->order)
		params[OFFSET_ORDER] =
		    pari_sprintf("%P0#*x", cfg->hex_digits, curve->order);

	if (curve->points) {
		char *points[curve->npoints];
		size_t len = 0;
		for (size_t i = 0; i < curve->npoints; ++i) {
			point_t *point = curve->points[i];
			GEN x = field_elementi(gel(point->point, 1));
			GEN y = field_elementi(gel(point->point, 2));
			points[i] = pari_sprintf("%P0#*x,%P0#*x,%P#x", cfg->hex_digits, x,
			                         cfg->hex_digits, y, point->order);
			len += strlen(points[i]);
		}
		size_t lenn = sizeof(char) * (len + curve->npoints);
		params[OFFSET_POINTS] = pari_calloc(lenn);
		for (size_t i = 0; i < curve->npoints; ++i) {
			if (i > 0) strncat(params[OFFSET_POINTS], ",", lenn - 1);
			strncat(params[OFFSET_POINTS], points[i], lenn - 1);
			pari_free(points[i]);
		}
	}

	size_t len = 0;
	size_t count = 0;
	for (int i = OFFSET_FIELD; i < OFFSET_END; ++i) {
		if (params[i]) {
			len += strlen(params[i]);
			++count;
		}
	}
	size_t lenn = sizeof(char) * (len + count);
	char *result = try_calloc(lenn);

	for (int i = OFFSET_FIELD; i < OFFSET_END; ++i) {
		if (params[i]) {
			if (i > OFFSET_FIELD) strncat(result, ",", lenn - 1);
			strncat(result, params[i], lenn - 1);
			pari_free(params[i]);
		}
	}

	avma = ltop;
	return result;
}

char *output_scsv_separator(const config_t *cfg) { return output_malloc("\n"); }

char *output_scsv_begin(const config_t *cfg) { return NULL; }

char *output_scsv_end(const config_t *cfg) { return output_malloc("\n"); }

static JSON_Value *output_jjson(curve_t *curve, const config_t *cfg) {
	pari_sp ltop = avma;
	// root object/value is curve
	JSON_Value *root_value = json_value_init_object();
	JSON_Object *root_object = json_value_get_object(root_value);

	switch (cfg->field) {
		case FIELD_PRIME: {
			char *prime = pari_sprintf("%P0#*x", cfg->hex_digits, curve->field);
			json_object_dotset_string(root_object, "field.p", prime);
			pari_free(prime);
			break;
		}
		case FIELD_BINARY: {
			GEN field = field_params(curve->field);
			char *m = pari_sprintf("%P#x", gel(field, 1));
			char *e1 = pari_sprintf("%P#x", gel(field, 2));
			char *e2 = pari_sprintf("%P#x", gel(field, 3));
			char *e3 = pari_sprintf("%P#x", gel(field, 4));
			json_object_dotset_string(root_object, "field.m", m);
			json_object_dotset_string(root_object, "field.e1", e1);
			json_object_dotset_string(root_object, "field.e2", e2);
			json_object_dotset_string(root_object, "field.e3", e3);
			pari_free(m);
			pari_free(e1);
			pari_free(e2);
			pari_free(e3);
			break;
		}
		default:
			fprintf(err, "Error, field has unknown amount of elements.\n");
			exit(1);
	}

	char *a = pari_sprintf("%P0#*x", cfg->hex_digits, field_elementi(curve->a));
	json_object_set_string(root_object, "a", a);
	pari_free(a);
	char *b = pari_sprintf("%P0#*x", cfg->hex_digits, field_elementi(curve->b));
	json_object_set_string(root_object, "b", b);
	pari_free(b);
	char *order = pari_sprintf("%P0#*x", cfg->hex_digits, curve->order);
	json_object_set_string(root_object, "order", order);
	pari_free(order);
	if (curve->ngens) {
		JSON_Value *gens_value = json_value_init_array();
		JSON_Array *gens_array = json_value_get_array(gens_value);

		for (size_t i = 0; i < curve->ngens; ++i) {
			JSON_Value *point_value = json_value_init_object();
			JSON_Object *point_object = json_value_get_object(point_value);

			char *x = pari_sprintf(
			    "%P0#*x", cfg->hex_digits,
			    field_elementi(gel(curve->generators[i]->point, 1)));
			json_object_set_string(point_object, "x", x);
			pari_free(x);
			char *y = pari_sprintf(
			    "%P0#*x", cfg->hex_digits,
			    field_elementi(gel(curve->generators[i]->point, 2)));
			json_object_set_string(point_object, "y", y);
			pari_free(y);
			char *p_order = pari_sprintf("%P#x", curve->generators[i]->order);
			json_object_set_string(point_object, "order", p_order);
			pari_free(p_order);
			if (curve->generators[i]->cofactor) {
				char *cofactor =
				    pari_sprintf("%P#x", curve->generators[i]->cofactor);
				json_object_set_string(point_object, "cofactor", cofactor);
				pari_free(cofactor);
			}

			json_array_append_value(gens_array, point_value);
		}

		json_object_set_value(root_object, "generators", gens_value);
	}

	if (curve->npoints) {
		JSON_Value *points_value = json_value_init_array();
		JSON_Array *points_array = json_value_get_array(points_value);

		for (size_t i = 0; i < curve->npoints; ++i) {
			JSON_Value *point_value = json_value_init_object();
			JSON_Object *point_object = json_value_get_object(point_value);

			char *x =
			    pari_sprintf("%P0#*x", cfg->hex_digits,
			                 field_elementi(gel(curve->points[i]->point, 1)));
			json_object_set_string(point_object, "x", x);
			pari_free(x);
			char *y =
			    pari_sprintf("%P0#*x", cfg->hex_digits,
			                 field_elementi(gel(curve->points[i]->point, 2)));
			json_object_set_string(point_object, "y", y);
			pari_free(y);
			char *p_order = pari_sprintf("%P#x", curve->points[i]->order);
			json_object_set_string(point_object, "order", p_order);
			pari_free(p_order);
			if (curve->points[i]->cofactor) {
				char *cofactor =
				    pari_sprintf("%P#x", curve->points[i]->cofactor);
				json_object_set_string(point_object, "cofactor", cofactor);
				pari_free(cofactor);
			}

			json_array_append_value(points_array, point_value);
		}

		json_object_set_value(root_object, "points", points_value);
	}
	avma = ltop;
	return root_value;
}

char *output_sjson(curve_t *curve, const config_t *cfg) {
	JSON_Value *root_value = output_jjson(curve, cfg);
	char *result = json_serialize_to_string_pretty(root_value);
	json_value_free(root_value);

	return result;
}

char *output_sjson_separator(const config_t *cfg) {
	return output_malloc(",\n");
}

char *output_sjson_begin(const config_t *cfg) { return output_malloc("[\n"); }

char *output_sjson_end(const config_t *cfg) { return output_malloc("]\n"); }

void output_f(FILE *out, curve_t *curve, const config_t *cfg) {
	char *s = output_s(curve, cfg);
	if (s) {
		fprintf(out, "%s", s);
		try_free(s);
	}
}

void output_o(curve_t *curve, const config_t *cfg) {
	output_f(out, curve, cfg);
}

void output_f_separator(FILE *out, const config_t *cfg) {
	char *s = output_s_separator(cfg);
	if (s) {
		fprintf(out, "%s", s);
		try_free(s);
	}
}

void output_o_separator(const config_t *cfg) { output_f_separator(out, cfg); }

void output_f_begin(FILE *out, const config_t *cfg) {
	char *s = output_s_begin(cfg);
	if (s) {
		fprintf(out, "%s", s);
		try_free(s);
	}
}

void output_o_begin(const config_t *cfg) { output_f_begin(out, cfg); }

void output_f_end(FILE *out, const config_t *cfg) {
	char *s = output_s_end(cfg);
	if (s) {
		fprintf(out, "%s", s);
		try_free(s);
	}
}

void output_o_end(const config_t *cfg) { output_f_end(out, cfg); }

bool output_init(const config_t *cfg) {
	json_set_allocation_functions(try_malloc, try_free);

	if (cfg->output) {
		out = fopen(cfg->output, cfg->append ? "a" : "w");
		if (!out) {
			// fallback to stdout and output err
			out = stdout;
			perror("Failed to open output file.");
		}
	} else {
		out = stdout;
	}
	setvbuf(out, NULL, _IONBF, 0);

	err = stderr;
	setvbuf(err, NULL, _IONBF, 0);

	if (cfg->verbose_log) {
		verbose = fopen(cfg->verbose_log, "w");
		if (!verbose) {
			verbose = stderr;
			perror("Failed to open verbose output file.");
		}
	} else {
		verbose = stderr;
	}
	setvbuf(verbose, NULL, _IONBF, 0);

	switch (cfg->format) {
		case FORMAT_JSON: {
			output_s = &output_sjson;
			output_s_separator = &output_sjson_separator;
			output_s_begin = &output_sjson_begin;
			output_s_end = &output_sjson_end;
			break;
		}
		case FORMAT_CSV: {
			output_s = &output_scsv;
			output_s_separator = &output_scsv_separator;
			output_s_begin = &output_scsv_begin;
			output_s_end = &output_scsv_end;
			break;
		}
	}
	return true;
}

void output_quit(void) {
	if (out != NULL && out != stdout) {
		fclose(out);
	}
	if (err != NULL && err != stdout) {
		fclose(err);
	}
}
