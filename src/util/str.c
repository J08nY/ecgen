/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "str.h"
#include <ctype.h>
#include <string.h>
#include "util/memory.h"

const char *str_is_hex(const char *hex_str) {
	const char *str_start = hex_str;
	if (strlen(hex_str) > 2) {
		if (hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X')) {
			str_start = hex_str + 2;
		}
	}
	const char *s = str_start;
	while (*s != 0) {
		char c = *s++;
		if (!isxdigit(c)) return NULL;
	}
	return str_start;
}

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