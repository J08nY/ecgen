/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "check.h"
#include "util/memory.h"

check_t *check_new(void) { return try_calloc(sizeof(check_t)); }

void check_free(check_t **check) {
	if (*check) {
		try_free(*check);
		*check = NULL;
	}
}