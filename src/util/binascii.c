/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "binascii.h"
#include "util/memory.h"

size_t binascii_blen(GEN i) {
	pari_sp ltop = avma;
	size_t result = (size_t)glength(binary_2k_nv(i, 8));
	avma = ltop;
	return result;
}

char *binascii_itob(GEN i, endian_e endianity) {
	pari_sp ltop = avma;
	GEN digits = binary_2k_nv(i, 8);
	if (endianity == ENDIAN_LITTLE) {
		digits = vecsmall_reverse(digits);
	}
	long blen = glength(digits);
	char *result = try_malloc((size_t)blen);
	for (long j = 1; j <= blen; ++j) {
		result[j - 1] = (char)gel(digits, j);
	}

	avma = ltop;
	return result;
}

GEN binascii_btoi(const char *bytes, size_t len, endian_e endianity) {
	pari_sp ltop = avma;
	GEN result = gen_0;
	for (size_t i = 0; i < len; ++i) {
		size_t index = i;
		if (endianity == ENDIAN_LITTLE) {
			index = (len - 1) - i;
		}
		result = addis(result, bytes[index]);
		if (i < len - 1) result = shifti(result, 8);
	}
	return gerepileupto(ltop, result);
}
