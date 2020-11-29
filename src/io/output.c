/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */

#include "output.h"
#include <parson/parson.h>
#include "gen/field.h"
#include "util/bits.h"
#include "util/memory.h"

char *(*output_s)(curve_t *curve);
char *(*output_s_separator)();
char *(*output_s_begin)();
char *(*output_s_end)();

FILE *out;
FILE *err;
FILE *verbose;

char *output_malloc(const char *what) {
	char *s = try_calloc(sizeof(char) * (strlen(what) + 1));
	strcpy(s, what);
	return s;
}

static JSON_Value *output_json_point(point_t *point) {
	JSON_Value *point_value = json_value_init_object();
	JSON_Object *point_object = json_value_get_object(point_value);

	char *x = pari_sprintf("%P0#*x", cfg->hex_digits,
	                       field_elementi(gel(point->point, 1)));
	json_object_set_string(point_object, "x", x);
	pari_free(x);
	char *y = pari_sprintf("%P0#*x", cfg->hex_digits,
	                       field_elementi(gel(point->point, 2)));
	json_object_set_string(point_object, "y", y);
	pari_free(y);
	char *p_order = pari_sprintf("%P#x", point->order);
	json_object_set_string(point_object, "order", p_order);
	pari_free(p_order);
	if (point->cofactor) {
		char *cofactor = pari_sprintf("%P#x", point->cofactor);
		json_object_set_string(point_object, "cofactor", cofactor);
		pari_free(cofactor);
	}

	return point_value;
}

static JSON_Value *output_jjson(curve_t *curve) {
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
	if (curve->seed) {
		char *hex_str = bits_to_hex(curve->seed->seed);
		char *hex = try_calloc(strlen(hex_str) + 3);
		hex[0] = '0';
		hex[1] = 'x';
		strcat(hex, hex_str);
		json_object_set_string(root_object, "seed", hex);
		try_free(hex_str);
		try_free(hex);
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
			JSON_Value *gen_value =
			    output_json_point(curve->generators[i]->generator);
			JSON_Object *gen_object = json_value_get_object(gen_value);

			if (curve->generators[i]->npoints) {
				JSON_Value *gens_points_value = json_value_init_array();
				JSON_Array *gens_points_array =
				    json_value_get_array(gens_points_value);
				for (size_t j = 0; j < curve->generators[i]->npoints; ++j) {
					json_array_append_value(
					    gens_points_array,
					    output_json_point(curve->generators[i]->points[j]));
				}
				json_object_set_value(gen_object, "points", gens_points_value);
			}
			json_array_append_value(gens_array, gen_value);
		}
		json_object_set_value(root_object, "subgroups", gens_value);
	}
	if (cfg->metadata) {
		if (curve->meta.j_invariant != NULL) {
			char *j = pari_sprintf("%Pi", curve->meta.j_invariant);
			json_object_dotset_string(root_object, "meta.j", j);
			pari_free(j);
		}
		if (curve->meta.discriminant != NULL) {
			char *disc = pari_sprintf("%Pi", curve->meta.discriminant);
			json_object_dotset_string(root_object, "meta.discriminant", disc);
			pari_free(disc);
		}
		if (curve->meta.embedding_degree != NULL) {
			char *embedding_degree =
			    pari_sprintf("%Pi", curve->meta.embedding_degree);
			json_object_dotset_string(root_object, "meta.embedding_degree",
			                          embedding_degree);
			pari_free(embedding_degree);
		}
		if (curve->meta.frobenius_trace != NULL) {
			char *frobenius = pari_sprintf("%Pi", curve->meta.frobenius_trace);
			json_object_dotset_string(root_object, "meta.frobenius", frobenius);
			pari_free(frobenius);
		}
		if (curve->meta.cm_discriminant != NULL) {
			char *cm_discriminant =
			    pari_sprintf("%Pi", curve->meta.cm_discriminant);
			json_object_dotset_string(root_object, "meta.cm_discriminant",
			                          cm_discriminant);
			pari_free(cm_discriminant);
		}
		if (curve->meta.conductor != NULL) {
			char *conductor = pari_sprintf("%Pi", curve->meta.conductor);
			json_object_dotset_string(root_object, "meta.conductor",
			                          conductor);
			pari_free(conductor);
		}
	}

	avma = ltop;
	return root_value;
}

char *output_sjson(curve_t *curve) {
	JSON_Value *root_value = output_jjson(curve);
	char *result = json_serialize_to_string_pretty(root_value);
	json_value_free(root_value);

	return result;
}

char *output_sjson_separator() { return output_malloc(",\n"); }

char *output_sjson_begin() { return output_malloc("[\n"); }

char *output_sjson_end() { return output_malloc("]\n"); }

void output_f(FILE *out, curve_t *curve) {
	char *s = output_s(curve);
	if (s) {
		fprintf(out, "%s", s);
		try_free(s);
	}
}

void output_o(curve_t *curve) { output_f(out, curve); }

void output_f_separator(FILE *out) {
	char *s = output_s_separator();
	if (s) {
		fprintf(out, "%s", s);
		try_free(s);
	}
}

void output_o_separator() { output_f_separator(out); }

void output_f_begin(FILE *out) {
	char *s = output_s_begin();
	if (s) {
		fprintf(out, "%s", s);
		try_free(s);
	}
}

void output_o_begin() { output_f_begin(out); }

void output_f_end(FILE *out) {
	char *s = output_s_end();
	if (s) {
		fprintf(out, "%s", s);
		try_free(s);
	}
}

void output_o_end() { output_f_end(out); }

bool output_init() {
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
	}
	return true;
}

static bool output_is_std(FILE *stream) {
	return (stream == stdout || stream == stderr || stream == stdin);
}

void output_quit(void) {
	if (!output_is_std(out)) {
		fclose(out);
	}
	if (!output_is_std(err)) {
		fclose(err);
	}
	if (!output_is_std(verbose)) {
		fclose(verbose);  // My name is fClose Verbose!
	}
}
