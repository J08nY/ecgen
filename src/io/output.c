/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include "output.h"
#include <parson/parson.h>
#include "math/curve.h"
#include "math/field.h"

FILE *out;
FILE *debug;

char *output_scsv(curve_t *curve, config_t *config) {
	pari_sp ltop = avma;
	GEN vector = curve_params(curve);

	long len = glength(vector);
	char *params[len];
	size_t lengths[len];
	size_t total = 0;
	for (long i = 0; i < len; ++i) {
		params[i] = pari_sprintf("%P#x", gel(vector, i + 1));
		lengths[i] = strlen(params[i]);
		total += lengths[i];
	}

	char *result = (char *)malloc(total + len);
	if (!result) {
		perror("Couldn't malloc.");
		exit(1);
	}

	size_t offset = 0;
	for (long i = 0; i < len; ++i) {
		memcpy(result + offset, params[i], lengths[i]);
		free(params[i]);

		offset += lengths[i];
		if (i != len - 1) {
			result[offset] = ',';
			offset++;
		}
	}
	memset(result + offset, 0, 1);

	avma = ltop;
	return result;
}

void output_fcsv(FILE *out, curve_t *curve, config_t *config) {
	char *string = output_scsv(curve, config);
	fprintf(out, "%s\n", string);
	free(string);
}

void output_csv(curve_t *curve, config_t *config) {
	output_fcsv(out, curve, config);
}

JSON_Value *output_jjson(curve_t *curve, config_t *config) {
	pari_sp ltop = avma;
	// root object/value is curve
	JSON_Value *root_value = json_value_init_object();
	JSON_Object *root_object = json_value_get_object(root_value);

	switch (config->field) {
		case FIELD_PRIME: {
			char *prime = pari_sprintf("%P#x", curve->field);
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
			fprintf(stderr, "Error, field has unknown amount of elements.\n");
			exit(1);
	}

	char *a = pari_sprintf("%P#x", field_elementi(curve->a));
	json_object_set_string(root_object, "a", a);
	pari_free(a);
	char *b = pari_sprintf("%P#x", field_elementi(curve->b));
	json_object_set_string(root_object, "b", b);
	pari_free(b);
	char *order = pari_sprintf("%P#x", curve->order);
	json_object_set_string(root_object, "order", order);
	pari_free(order);
	if (curve->generators) {
		JSON_Value *gens_value = json_value_init_array();
		JSON_Array *gens_array = json_value_get_array(gens_value);

		for (size_t i = 0; i < curve->ngens; ++i) {
			JSON_Value *point_value = json_value_init_object();
			JSON_Object *point_object = json_value_get_object(point_value);

			char *x = pari_sprintf(
			    "%P#x", field_elementi(gel(curve->generators[i]->point, 1)));
			json_object_set_string(point_object, "x", x);
			pari_free(x);
			char *y = pari_sprintf(
			    "%P#x", field_elementi(gel(curve->generators[i]->point, 2)));
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

			char *x = pari_sprintf(
			    "%P#x", field_elementi(gel(curve->points[i]->point, 1)));
			json_object_set_string(point_object, "x", x);
			pari_free(x);
			char *y = pari_sprintf(
			    "%P#x", field_elementi(gel(curve->points[i]->point, 2)));
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

char *output_sjson(curve_t *curve, config_t *config) {
	JSON_Value *root_value = output_jjson(curve, config);
	char *result = json_serialize_to_string_pretty(root_value);
	json_value_free(root_value);

	return result;
}

void output_fjson(FILE *out, curve_t *curve, config_t *config) {
	char *s = output_sjson(curve, config);
	fprintf(out, "%s\n", s);
	json_free_serialized_string(s);
}

void output_json(curve_t *curve, config_t *config) {
	output_fjson(out, curve, config);
}

void output_init(config_t *cfg) {
	json_set_allocation_functions(pari_malloc, pari_free);

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
	if (cfg->debug) {
		debug = fopen(cfg->debug, "w");
		if (!debug) {
			debug = stdout;
			perror("Failed to open verbose output file.");
		}
	} else {
		debug = stdout;
	}
	setvbuf(debug, NULL, _IONBF, 0);

	switch (cfg->format) {
		case FORMAT_JSON:
			output_s = &output_sjson;
			output_f = &output_fjson;
			output_o = &output_json;
			break;
		case FORMAT_CSV:
			output_s = &output_scsv;
			output_f = &output_fcsv;
			output_o = &output_csv;
			break;
	}
}

void output_quit(void) {
	if (out != NULL && out != stdout) {
		fclose(out);
	}
}
