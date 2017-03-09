/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "arg.h"

arg_t *arg_new() {
	arg_t *arg = pari_malloc(sizeof(arg_t));
	if (!arg) {
		perror("Couldn't malloc.");
		exit(1);
	}
	memset(arg, 0, sizeof(arg_t));
	return arg;
}

void arg_free(arg_t **arg) {
	if (*arg) {
		pari_free(*arg);
		*arg = NULL;
	}
}
