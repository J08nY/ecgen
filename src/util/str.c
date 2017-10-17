/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "str.h"
#include <string.h>
#include "util/memory.h"

char *str_join(char *strings[], size_t len) {
	size_t total = 0;
	for (size_t i = 0; i < len; ++i) {
		if (strings[i]) total += strlen(strings[i]);
	}
	char *result = try_calloc(total);
	for (size_t i = 0; i < len; ++i) {
		if (strings[i]) {
			strcat(result, strings[i]);
		}
	}
	return result;
}