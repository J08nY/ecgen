/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file bits.h
 */
#ifndef ECGEN_UTIL_BITS_H
#define ECGEN_UTIL_BITS_H

#include "misc/types.h"

#define BYTE_LEN(bit_len) \
	(((bit_len) % 8 == 0) ? (bit_len) / 8 : ((bit_len) / 8) + 1)

#define GET_BIT(bit_array, bit_pos)                                 \
	(((bit_array)[(bit_pos) / 8] & (1 << (7 - ((bit_pos) % 8)))) >> \
	 (7 - ((bit_pos) % 8)))

#define SET_BIT(bit_array, bit_pos, bit_value)          \
	do {                                                \
		unsigned char val = 1 << (7 - ((bit_pos) % 8)); \
		if ((bit_value) == 1) {                         \
			(bit_array)[(bit_pos) / 8] |= val;          \
		} else {                                        \
			(bit_array)[(bit_pos) / 8] &= ~val;         \
		}                                               \
	} while (0);

bits_t *bits_new(size_t bit_len);

bits_t *bits_new_rand(size_t bit_len);

void bits_cpy(bits_t *dest, const bits_t *src);

bits_t *bits_copy(const bits_t *bits);

void bits_free(bits_t **bits);

bits_t *bits_from_i(GEN i);

bits_t *bits_from_i_len(GEN i, size_t bitlen);

bits_t *bits_from_hex(const char *hex_str);

bits_t *bits_from_bin(const char *bin_str);

bits_t *bits_from_raw(const unsigned char *bits, size_t bit_len);

bits_t *bits_from_bitvec(GEN v);

GEN bits_to_i(const bits_t *bits);

char *bits_to_hex(const bits_t *bits);

char *bits_to_bin(const bits_t *bits);

unsigned char *bits_to_raw(const bits_t *bits);

size_t bits_to_rawlen(const bits_t *bits);

GEN bits_to_bitvec(const bits_t *bits);

void bits_concatz(bits_t *one, ...);

bits_t *bits_concat(const bits_t *one, ...);

bits_t *bits_or(const bits_t *one, const bits_t *other);

bits_t *bits_and(const bits_t *one, const bits_t *other);

void bits_notz(bits_t *bits);

bits_t *bits_not(const bits_t *bits);

void bits_rotz(bits_t *bits);

bits_t *bits_rot(const bits_t *bits);

void bits_shiftz(bits_t *bits, long amount);

bits_t *bits_shift(const bits_t *bits, long amount);

void bits_shiftrz(bits_t *bits, long amount);

bits_t *bits_shiftr(const bits_t *bits, long amount);

void bits_shiftiz(bits_t *bits, long amount);

bits_t *bits_shifti(const bits_t *bits, long amount);

void bits_lengthenz(bits_t *bits, long amount);

bits_t *bits_lengthen(const bits_t *bits, long amount);

void bits_shortenz(bits_t *bits, long amount);

bits_t *bits_shorten(const bits_t *bits, long amount);

void bits_sha1(const bits_t *bits, unsigned char hashout[20]);

bool bits_eq(const bits_t *one, const bits_t *other);

#endif  // ECGEN_UTIL_BITS_H
