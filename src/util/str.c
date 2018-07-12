/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "str.h"
#include <ctype.h>
#include <stdarg.h>
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

char *str_join(char *separator, char **strings, size_t len) {
	size_t total = 1;
	for (size_t i = 0; i < len; ++i) {
		if (strings[i]) total += strlen(strings[i]);
	}
	if (separator) total += (len - 1) * strlen(separator);
	char *result = try_calloc(total);
	for (size_t i = 0; i < len; ++i) {
		if (strings[i]) {
			strcat(result, strings[i]);
		}
		if (separator && i != len - 1) {
			strcat(result, separator);
		}
	}
	return result;
}

char *str_joinv(char *separator, ...) {
	va_list valist;
	va_start(valist, separator);

	size_t len = 5;
	char **strings = try_malloc(len * sizeof(char *));
	char *next;
	size_t i = 0;
	while ((next = va_arg(valist, char *)) != NULL) {
		if (i == len) {
			strings = try_realloc(strings, (len * 2) * sizeof(char *));
		}
		strings[i++] = next;
	}

	va_end(valist);
	char *result = str_join(separator, strings, i);
	try_free(strings);
	return result;
}

char *str_concat(char **strings, size_t len) {
	return str_join(NULL, strings, len);
}

size_t str_cnt(const char *str, const char c) {
	size_t result = 0;
	for (; str[result]; str[result] == c ? result++ : *str++)
		;
	return result;
}
