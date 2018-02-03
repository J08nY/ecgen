/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "check.h"
#include "util/memory.h"

check_t *check_new(check_f one, ...) {
	check_t *result = try_calloc(sizeof(check_t));
	result->checks = try_calloc(sizeof(check_f));
	result->checks[0] = one;
	result->nchecks = 1;

	va_list args;
	va_start(args, one);
	check_f check;
	while ((check = va_arg(args, check_f)) != NULL) {
		result->checks =
		    try_realloc(result->checks, sizeof(check_f) * (++result->nchecks));
		result->checks[result->nchecks - 1] = check;
	}
	va_end(args);

	return result;
}

void check_free(check_t **check) {
	if (*check) {
		if ((*check)->checks) {
			try_free((*check)->checks);
		}
		try_free(*check);
		*check = NULL;
	}
}