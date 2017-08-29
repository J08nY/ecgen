/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "utils.h"
#include <pari/pari.h>

void default_setup(void) { pari_init(1000000, 1000000); }

void default_teardown(void) { pari_close(); }