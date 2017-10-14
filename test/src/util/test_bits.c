/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <criterion/criterion.h>
#include <criterion/parameterized.h>
#include "test/default.h"
#include "test/memory.h"
#include "util/bits.h"
#include "util/memory.h"

TestSuite(bits, .init = default_setup, .fini = default_teardown);

Test(bits, test_bits_new) {
	bits_t *bits = bits_new(10);
	cr_assert_not_null(bits, );
	cr_assert_eq(bits->bitlen, 10, );
	cr_assert_eq(bits->allocated, 2, );
	cr_assert_eq(bits->bits[0], 0, );
	cr_assert_eq(bits->bits[1], 0, );
	bits_free(&bits);
}

Test(bits, test_bits_copy) {
	bits_t *bits = bits_new(10);
	bits->bits[0] = 0b10101010;
	bits->bits[1] = 0b11000000;

	bits_t *other_bits = bits_copy(bits);
	cr_assert_not_null(other_bits, );
	cr_assert_eq(other_bits->allocated, bits->allocated, );
	cr_assert_eq(other_bits->bitlen, bits->bitlen, );
	cr_assert_eq(other_bits->bits[0], bits->bits[0], );
	cr_assert_eq(other_bits->bits[1], bits->bits[1], );
	bits_free(&bits);
	bits_free(&other_bits);
}

Test(bits, test_bits_from_i) {
	GEN i = int2n(5);

	bits_t *bits = bits_from_i(i);
	cr_assert_not_null(bits, );
	cr_assert_eq(bits->bitlen, 6, );
	cr_assert_eq(bits->allocated, 1, );
	cr_assert_eq(bits->bits[0], 0b10000000, );
	bits_free(&bits);
}

Test(bits, test_bits_from_hex) {
	char *hex = "0ab";

	bits_t *bits = bits_from_hex(hex);
	cr_assert_not_null(bits, );
	cr_assert_eq(bits->bitlen, 12, );
	cr_assert_eq(bits->allocated, 2, );
	cr_assert_eq(bits->bits[0], 0x0a, );
	cr_assert_eq(bits->bits[1], 0xb0, );
	bits_free(&bits);
}

Test(bits, test_bits_from_bin) {
	char *bin = "000010101101";

	bits_t *bits = bits_from_bin(bin);
	cr_assert_not_null(bits, );
	cr_assert_eq(bits->bitlen, 12, );
	cr_assert_eq(bits->allocated, 2, );
	cr_assert_eq(bits->bits[0], 0b00001010, );
	cr_assert_eq(bits->bits[1], 0b11010000, );
	bits_free(&bits);
}

Test(bits, test_bits_from_raw) {
	unsigned char *raw = (unsigned char *)"\x0a\xb0";

	bits_t *bits = bits_from_raw(raw, 16);
	cr_assert_not_null(bits, );
	cr_assert_eq(bits->bitlen, 16, );
	cr_assert_eq(bits->allocated, 2, );
	cr_assert_eq(bits->bits[0], 0x0a, );
	cr_assert_eq(bits->bits[1], 0xb0, );
	bits_free(&bits);
}

Test(bits, test_bits_from_bitvec) {
	GEN bitvec = binary_zv(int2n(5));

	bits_t *bits = bits_from_bitvec(bitvec);
	cr_assert_not_null(bits, );
	cr_assert_eq(bits->bitlen, 6, );
	cr_assert_eq(bits->allocated, 1, );
	cr_assert_eq(bits->bits[0], 0b10000000, );
	bits_free(&bits);
}

Test(bits, test_bits_to_i) {
	bits_t *bits = bits_new(6);
	bits->bits[0] = 0b10000000;

	GEN i = bits_to_i(bits);
	cr_assert_not_null(bits, );
	cr_assert(gequal(i, int2n(5)), );
	bits_free(&bits);
}

Test(bits, test_bits_to_hex) {
	bits_t *bits = bits_new(12);
	bits->bits[0] = 0xab;
	bits->bits[1] = 0xc0;

	char *hex = bits_to_hex(bits);
	cr_assert_not_null(hex, );
	cr_assert_str_eq(hex, "abc0", );
	try_free(hex);
	bits_free(&bits);
}

Test(bits, test_bits_to_bin) {
	bits_t *bits = bits_new(12);
	bits->bits[0] = 0b10101010;
	bits->bits[1] = 0b11110000;

	char *bin = bits_to_bin(bits);
	cr_assert_not_null(bin, );
	cr_assert_str_eq(bin, "101010101111", );
	try_free(bin);
	bits_free(&bits);
}

Test(bits, test_bits_to_raw) {
	bits_t *bits = bits_new(6);
	bits->bits[0] = 0b10000000;

	unsigned char *raw = bits_to_raw(bits);
	size_t rawlen = bits_to_rawlen(bits);
	cr_assert_not_null(raw, );
	cr_assert_eq(rawlen, 1, );
	cr_assert_eq(raw[0], 0b10000000, );
	try_free(raw);
	bits_free(&bits);
}

Test(bits, test_bits_to_bitvec) {
	bits_t *bits = bits_new(6);
	bits->bits[0] = 0b10000000;

	GEN bitvec = bits_to_bitvec(bits);
	cr_assert_not_null(bitvec, );
	cr_assert(gequal(bitvec, binary_zv(int2n(5))), );
}

Test(bits, test_bits_concat) {
	bits_t *one = bits_new(6);
	one->bits[0] = 0b00000000;

	bits_t *other = bits_new(8);
	other->bits[0] = 0b11001100;

	bits_t *cat = bits_concat(one, other, NULL);
	cr_assert_not_null(cat, );
	cr_assert_eq(cat->bitlen, 14, );
	cr_assert_eq(cat->bits[0], 0b00000011, );
	cr_assert_eq(cat->bits[1], 0b00110000, );

	bits_free(&one);
	bits_free(&other);
	bits_free(&cat);
}

Test(bits, test_bits_or) {
	bits_t *bits = bits_new(6);
	bits->bits[0] = 0b10000000;

	bits_t *other_bits = bits_new(10);
	other_bits->bits[0] = 0b00000000;
	other_bits->bits[1] = 0b11000000;

	bits_t * or = bits_or(bits, other_bits);
	cr_assert_not_null(or, );
	cr_assert_eq(or->bitlen, 10, );
	cr_assert_eq(or->bits[0], 0b00001000, );
	cr_assert_eq(or->bits[1], 0b11000000, );
	bits_free(&bits);
	bits_free(&other_bits);
	bits_free(& or);
}

Test(bits, test_bits_and) {
	bits_t *bits = bits_new(6);
	bits->bits[0] = 0b10000000;

	bits_t *other_bits = bits_new(10);
	other_bits->bits[0] = 0b00001000;
	other_bits->bits[1] = 0b11000000;

	bits_t *and = bits_and(bits, other_bits);
	cr_assert_not_null(and, );
	cr_assert_eq(and->bitlen, 10, );
	cr_assert_eq(and->bits[0], 0b00001000, );
	cr_assert_eq(and->bits[1], 0b00000000, );
	bits_free(&bits);
	bits_free(&other_bits);
	bits_free(&and);
}

Test(bits, test_bits_notz) {
	bits_t *bits = bits_new(6);
	bits->bits[0] = 0b10000000;

	bits_notz(bits);
	cr_assert_eq(bits->bitlen, 6, );
	cr_assert_eq(bits->bits[0], 0b01111100, );
	bits_free(&bits);
}

Test(bits, test_bits_not) {
	bits_t *bits = bits_new(6);
	bits->bits[0] = 0b10000000;

	bits_t *not = bits_not(bits);
	cr_assert_not_null(not, );
	cr_assert_eq(not->bitlen, 6, );
	cr_assert_eq(not->bits[0], 0b01111100, );
	bits_free(&bits);
	bits_free(&not);
}

Test(bits, test_bits_rotz) {
	bits_t *bits = bits_new(10);
	bits->bits[0] = 0b11111000;
	bits->bits[1] = 0b00000000;

	bits_rotz(bits);
	cr_assert_eq(bits->bitlen, 10, );
	cr_assert_eq(bits->bits[0], 0b00000111, );
	cr_assert_eq(bits->bits[1], 0b11000000, );
	bits_free(&bits);
}

Test(bits, test_bits_rot) {
	bits_t *bits = bits_new(6);
	bits->bits[0] = 0b10000000;

	bits_t *rot = bits_rot(bits);
	cr_assert_not_null(rot, );
	cr_assert_eq(rot->bitlen, 6, );
	cr_assert_eq(rot->bits[0], 0b00000100, );
	bits_free(&bits);
	bits_free(&rot);

	bits = bits_new(5);
	bits->bits[0] = 0b10100000;

	rot = bits_rot(bits);
	cr_assert_not_null(rot, );
	cr_assert_eq(rot->bitlen, 5, );
	cr_assert_eq(rot->bits[0], 0b00101000, );
	bits_free(&bits);
	bits_free(&rot);
}

Test(bits, test_bits_shiftz) {
	bits_t *bits = bits_new(6);
	bits->bits[0] = 0b10101000;

	bits_shiftz(bits, -1);
	cr_assert_eq(bits->bitlen, 6, );
	cr_assert_eq(bits->bits[0], 0b01010100, );
	bits_free(&bits);
}

Test(bits, test_bits_shift) {
	bits_t *bits = bits_new(8);
	bits->bits[0] = 0b11001100;

	bits_t *shift = bits_shift(bits, 2);
	cr_assert_not_null(shift, );
	cr_assert_eq(shift->bitlen, 8, );
	cr_assert_eq(shift->bits[0], 0b00110000, );
	bits_free(&bits);
	bits_free(&shift);
}

Test(bits, test_bits_shiftrz) {
	bits_t *bits = bits_new(8);
	bits->bits[0] = 0b11001100;

	bits_shiftrz(bits, 1);
	cr_assert_eq(bits->bitlen, 8, );
	cr_assert_eq(bits->bits[0], 0b10011001, );
	bits->bits[0] = 0b11001100;

	bits_shiftrz(bits, 9);
	cr_assert_eq(bits->bitlen, 8, );
	cr_assert_eq(bits->bits[0], 0b10011001, );
	bits->bits[0] = 0b11001100;

	bits_shiftrz(bits, -10);
	cr_assert_eq(bits->bitlen, 8, );
	cr_assert_eq(bits->bits[0], 0b00110011, );
	bits_free(&bits);
}

Test(bits, test_bits_shiftr) {
	bits_t *bits = bits_new(8);
	bits->bits[0] = 0b11001100;

	bits_t *shift = bits_shiftr(bits, 1);
	cr_assert_eq(shift->bitlen, 8, );
	cr_assert_eq(shift->bits[0], 0b10011001, );
	bits_free(&bits);
	bits_free(&shift);
}

Test(bits, test_bits_shiftiz) {
	bits_t *bits = bits_new(8);
	bits->bits[0] = 0b11001100;

	bits_shiftiz(bits, 2);
	cr_assert_eq(bits->bitlen, 10, );
	cr_assert_eq(bits->bits[0], 0b11001100, );
	cr_assert_eq(bits->bits[1], 0b00000000, );

	bits_shiftiz(bits, -2);
	cr_assert_eq(bits->bitlen, 8, );
	cr_assert_eq(bits->bits[0], 0b11001100, );
	bits_free(&bits);
}

Test(bits, test_bits_shitfi) {
	bits_t *bits = bits_new(8);
	bits->bits[0] = 0b11001100;

	bits_t *shift = bits_shifti(bits, -4);
	cr_assert_eq(shift->bitlen, 4, );
	cr_assert_eq(shift->bits[0], 0b11000000, );
	bits_free(&bits);
	bits_free(&shift);
}

Test(bits, test_bits_lengthenz) {
	bits_t *bits = bits_new(8);
	bits->bits[0] = 0b11001100;

	bits_lengthenz(bits, 4);
	cr_assert_eq(bits->bitlen, 12, );
	cr_assert_eq(bits->bits[0], 0b00001100, );
	cr_assert_eq(bits->bits[1], 0b11000000, );

	bits_lengthenz(bits, -4);
	cr_assert_eq(bits->bitlen, 16, );
	cr_assert_eq(bits->bits[0], 0b00001100, );
	cr_assert_eq(bits->bits[1], 0b11000000, );

	bits_free(&bits);
}

Test(bits, test_bits_lengthen) {
	bits_t *bits = bits_new(6);
	bits->bits[0] = 0b11111100;

	bits_t *longer = bits_lengthen(bits, 2);
	cr_assert_not_null(longer, );
	cr_assert_eq(longer->bitlen, 8, );
	cr_assert_eq(longer->bits[0], 0b00111111, );
	bits_free(&bits);
	bits_free(&longer);
}

Test(bits, test_bits_shortenz) {
	bits_t *bits = bits_new(6);
	bits->bits[0] = 0b10110100;

	bits_shortenz(bits, 2);
	cr_assert_eq(bits->bitlen, 4, );
	cr_assert_eq(bits->bits[0], 0b11010000, );

	bits_shortenz(bits, -2);
	cr_assert_eq(bits->bitlen, 2, );
	cr_assert_eq(bits->bits[0], 0b11000000, );
	bits_free(&bits);
}

Test(bits, test_bits_shorten) {
	bits_t *bits = bits_new(6);
	bits->bits[0] = 0b10110100;

	bits_t *shorter = bits_shorten(bits, 4);
	cr_assert_not_null(shorter, );
	cr_assert_eq(shorter->bitlen, 2, );
	cr_assert_eq(shorter->bits[0], 0b01000000, );
	bits_free(&bits);
	bits_free(&shorter);
}

struct sha1_params {
	char *data;
	unsigned char *hashout;
};

void sha1_params_cleanup(struct criterion_test_params *ctp) {
	struct sha1_params *params = (struct sha1_params *)ctp->params;
	cr_free(params->data);
	cr_free(params->hashout);
}

ParameterizedTestParameters(bits, test_bits_sha1) {
	static struct sha1_params params[5] = {};
	// clang-format off
	params[0].data = cr_strdup("The quick brown fox jumps over the lazy dog");
	params[0].hashout = cr_memdup("\x2f\xd4\xe1\xc6\x7a\x2d\x28\xfc\xed\x84\x9e\xe1\xbb\x76\xe7\x39\x1b\x93\xeb\x12", 20);
	params[1].data = cr_strdup("abc");
	params[1].hashout = cr_memdup("\xa9\x99\x3e\x36\x47\x06\x81\x6a\xba\x3e\x25\x71\x78\x50\xc2\x6c\x9c\xd0\xd8\x9d", 20);
	params[2].data = cr_strdup("");
	params[2].hashout = cr_memdup("\xda\x39\xa3\xee\x5e\x6b\x4b\x0d\x32\x55\xbf\xef\x95\x60\x18\x90\xaf\xd8\x07\x09", 20);
	params[3].data = cr_strdup("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
	params[3].hashout = cr_memdup("\x84\x98\x3e\x44\x1c\x3b\xd2\x6e\xba\xae\x4a\xa1\xf9\x51\x29\xe5\xe5\x46\x70\xf1", 20);
	params[4].data = cr_strdup("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
	params[4].hashout = cr_memdup("\xa4\x9b\x24\x46\xa0\x2c\x64\x5b\xf4\x19\xf9\x95\xb6\x70\x91\x25\x3a\x04\xa2\x59", 20);
	// clang-format on

	size_t nb_params = sizeof(params) / sizeof(struct sha1_params);
	return cr_make_param_array(struct sha1_params, params, nb_params,
	                           sha1_params_cleanup);
}

ParameterizedTest(struct sha1_params *param, bits, test_bits_sha1) {
	bits_t *bits =
	    bits_from_raw((unsigned char *)param->data, strlen(param->data) * 8);
	unsigned char hashout[20] = {};
	bits_sha1(bits, hashout);

	for (size_t i = 0; i < 20; ++i) {
		cr_assert_eq(hashout[i], param->hashout[i], );
	}

	bits_free(&bits);
}

Test(bits, test_bits_eq) {
	bits_t *bits = bits_new(6);
	bits->bits[0] = 0b10000000;

	bits_t *other_bits = bits_new(6);
	other_bits->bits[0] = 0b10000000;

	cr_assert(bits_eq(bits, other_bits), );

	bits->bits[0] = 0b10010000;
	other_bits->bits[0] = 0b10000000;

	cr_assert_not(bits_eq(bits, other_bits), );

	bits_free(&bits);
	bits_free(&other_bits);
}

Test(bits, test_bits_eq_len) {
	bits_t *bits = bits_new(5);
	bits->bits[0] = 0b10000000;

	bits_t *other_bits = bits_new(6);
	other_bits->bits[0] = 0b10000000;
	cr_assert_not(bits_eq(bits, other_bits), );
	bits_free(&bits);
	bits_free(&other_bits);
}

Test(bits, test_bits_eq_large) {
	bits_t *bits = bits_new(10);
	bits->bits[0] = 0b10000010;
	bits->bits[1] = 0b11000000;

	bits_t *other_bits = bits_new(10);
	other_bits->bits[0] = 0b10000010;
	other_bits->bits[1] = 0b11000000;

	cr_assert(bits_eq(bits, other_bits), );

	bits->bits[0] = 0b10000000;
	other_bits->bits[0] = 0b00000010;

	cr_assert_not(bits_eq(bits, other_bits), );

	bits_free(&bits);
	bits_free(&other_bits);
}
