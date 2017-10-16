/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include <criterion/criterion.h>
#include "test/io.h"

TestSuite(order, .init = io_setup, .fini = io_teardown);