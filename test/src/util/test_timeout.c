/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <criterion/criterion.h>
#include "test/default.h"
#include "util/timeout.h"

void timeout_setup(void) {
	default_setup();
	timeout_init();
}

TestSuite(timeout, .init = timeout_setup, .fini = default_teardown);

Test(timeout, test_timeout_stop) {
	bool done = false;
	timeout_start(5) { cr_assert_fail(); }
	else {
		done = true;
	}
	timeout_stop();
	cr_assert(done, );
}

Test(timeout, test_timeout_handle) {
	bool done = false;
	timeout_start(1) { done = true; }
	else {
		sleep(2);
	}
	cr_assert(done, );
}