/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <criterion/criterion.h>
#include <pari/pari.h>
#include "test/default.h"
#include "util/binascii.h"
#include "util/memory.h"

TestSuite(binascii, .init = default_setup, .fini = default_teardown);

Test(binascii, test_itob_big) {
	GEN i = stoi((5 << 8) | 4);
	unsigned char *bytes = binascii_itob(i, ENDIAN_BIG);
	size_t len = binascii_blen(i);
	cr_assert_eq(len, 2,);
	cr_assert_eq(bytes[0], 5,);
	cr_assert_eq(bytes[1], 4,);
	try_free(bytes);
}

Test(binascii, test_itob_little) {
	GEN i = stoi((5 << 8) | 4);
	unsigned char *bytes = binascii_itob(i, ENDIAN_LITTLE);
	size_t len = binascii_blen(i);
	cr_assert_eq(len, 2,);
	cr_assert_eq(bytes[0], 4,);
	cr_assert_eq(bytes[1], 5,);
	try_free(bytes);
}

Test(binascii, test_btoi_big) {
	unsigned char bytes[] = {5, 4};
	GEN i = binascii_btoi(bytes, 2, ENDIAN_BIG);
	GEN expected = stoi((5 << 8) | 4);
	cr_assert(equalii(expected, i),);
}

Test(binascii, test_btoi_little) {
	unsigned char bytes[] = {5, 4};
	GEN i = binascii_btoi(bytes, 2, ENDIAN_LITTLE);
	GEN expected = stoi((4 << 8) | 5);
	cr_assert(equalii(expected, i),);
}

Test(binascii, test_both) {
	GEN expected = stoi((5 << 8) | 4);
	unsigned char *bytes = binascii_itob(expected, ENDIAN_BIG);
	size_t len = binascii_blen(expected);
	GEN i = binascii_btoi(bytes, len, ENDIAN_BIG);
	cr_assert_eq(len, 2,);
	cr_assert(equalii(expected, i),);
	try_free(bytes);

	unsigned char other_bytes[] = {5, 4, 3, 2, 1, 128};
	GEN j = binascii_btoi(other_bytes, 6, ENDIAN_BIG);
	unsigned char *result_bytes = binascii_itob(j, ENDIAN_BIG);
	size_t other_len = binascii_blen(j);
	cr_assert_eq(other_len, 6,);
	cr_assert_eq(result_bytes[0], 5,);
	cr_assert_eq(result_bytes[1], 4,);
	cr_assert_eq(result_bytes[2], 3,);
	cr_assert_eq(result_bytes[3], 2,);
	cr_assert_eq(result_bytes[4], 1,);
	cr_assert_eq(result_bytes[5], 128,);
	try_free(result_bytes);
}