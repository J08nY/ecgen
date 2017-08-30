/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "utils.h"
#include <criterion/criterion.h>
#include <pari/pari.h>

static void *cr_simple_calloc(size_t size) {
	return cr_calloc(1, size);
}

void default_setup(void) {
	pari_init(1000000, 1000000);
	//set_mem_funcs(cr_malloc, cr_simple_calloc, cr_realloc, cr_free);
}

void default_teardown(void) { pari_close(); }