/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <criterion/criterion.h>
#include "test/default.h"
#include "util/binascii.h"
#include "util/memory.h"

TestSuite(binascii, .init = default_setup, .fini = default_teardown);

Test(binascii, test_itob_big) {
	GEN i = stoi((5 << 8) | 4);
	char *bytes = binascii_itob(i, ENDIAN_BIG);
	size_t len = binascii_blen(i);
	cr_assert_eq(len, 2, );
	cr_assert_eq(bytes[0], 5, );
	cr_assert_eq(bytes[1], 4, );
	try_free(bytes);
}

Test(binascii, test_itob_little) {
	GEN i = stoi((5 << 8) | 4);
	char *bytes = binascii_itob(i, ENDIAN_LITTLE);
	size_t len = binascii_blen(i);
	cr_assert_eq(len, 2, );
	cr_assert_eq(bytes[0], 4, );
	cr_assert_eq(bytes[1], 5, );
	try_free(bytes);
}

Test(binascii, test_btoi_big) {
	char bytes[] = {5, 4};
	GEN i = binascii_btoi(bytes, 2, ENDIAN_BIG);
	GEN expected = stoi((5 << 8) | 4);
	cr_assert(equalii(expected, i), );
}

Test(binascii, test_btoi_little) {
	char bytes[] = {5, 4};
	GEN i = binascii_btoi(bytes, 2, ENDIAN_LITTLE);
	GEN expected = stoi((4 << 8) | 5);
	cr_assert(equalii(expected, i), );
}

Test(binascii, test_both) {
	GEN expected = stoi((5 << 8) | 4);
	char *bytes = binascii_itob(expected, ENDIAN_BIG);
	size_t len = binascii_blen(expected);
	GEN i = binascii_btoi(bytes, len, ENDIAN_BIG);
	cr_assert(equalii(expected, i), );
	try_free(bytes);
}