/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "arg.h"
#include "util/memory.h"

arg_t *arg_new(void) { return try_calloc(sizeof(arg_t)); }

void arg_free(arg_t **arg) {
	if (*arg) {
		if ((*arg)->allocd) {
			try_free((*arg)->allocd);
			(*arg)->allocd = NULL;
		}
		try_free(*arg);
		*arg = NULL;
	}
}
