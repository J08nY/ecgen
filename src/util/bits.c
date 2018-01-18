/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */

#include "bits.h"
#include <sha1/sha1.h>
#include "util/memory.h"

bits_t *bits_new(size_t bit_len) {
	bits_t *result = try_calloc(sizeof(bits_t));
	size_t byte_len = BYTE_LEN(bit_len);
	if (byte_len > 0) result->bits = try_calloc(byte_len);
	result->allocated = byte_len;
	result->bitlen = bit_len;
	return result;
}

bits_t *bits_new_rand(size_t bit_len) {
	bits_t *result = bits_new(bit_len);
	for (size_t i = 0; i < result->allocated; ++i) {
		if (i == result->allocated - 1) {
			size_t last_bits = bit_len % 8;
			result->bits[i] = (unsigned char)random_bits(last_bits)
			                  << (8 - last_bits);
		} else {
			result->bits[i] = (unsigned char)random_bits(8);
		}
	}
	return result;
}

void bits_cpy(bits_t *dest, const bits_t *src) {
	if (src->bitlen == 0) {
		return;
	}

	if (src->allocated < dest->allocated) {
		memset(dest->bits + src->allocated, 0,
		       dest->allocated - src->allocated);
	} else if (src->allocated > dest->allocated) {
		dest->bits = try_realloc(dest->bits, src->allocated);
	}
	memcpy(dest->bits, src->bits, src->allocated);
	dest->allocated = src->allocated;
	dest->bitlen = src->bitlen;
}

bits_t *bits_copy(const bits_t *bits) {
	bits_t *result = try_calloc(sizeof(bits_t));
	result->bitlen = bits->bitlen;
	result->allocated = bits->allocated;
	if (bits->allocated != 0)
		result->bits = try_memdup(bits->bits, result->allocated);
	return result;
}

void bits_free(bits_t **bits) {
	if (*bits) {
		if ((*bits)->bits) {
			try_free((*bits)->bits);
		}
		try_free(*bits);
		*bits = NULL;
	}
}

bits_t *bits_from_i(GEN i) {
	pari_sp ltop = avma;
	GEN bitvec = binary_zv(i);
	size_t bit_len = (size_t)glength(bitvec);
	bits_t *result = bits_new(bit_len);
	for (size_t j = 0; j < bit_len; ++j) {
		if (gel(bitvec, j + 1) == (GEN)1) {
			result->bits[j / 8] |= 1 << (7 - (j % 8));
		}
	}
	avma = ltop;
	return result;
}

bits_t *bits_from_i_len(GEN i, size_t bit_len) {
	pari_sp ltop = avma;
	GEN bitvec = binary_zv(i);
	size_t i_len = (size_t)glength(bitvec);
	bits_t *result = bits_new(bit_len);
	size_t offset = 0;
	if (i_len < bit_len) {
		offset = bit_len - i_len;
	}
	for (size_t j = 0; j < bit_len; ++j) {
		if (j < i_len && gel(bitvec, j + 1) == (GEN)1) {
			result->bits[(j + offset) / 8] |= 1 << (7 - ((j + offset) % 8));
		}
	}
	avma = ltop;
	return result;
}

bits_t *bits_from_hex(const char *hex_str) {
	size_t nibble_len = strlen(hex_str);
	bits_t *result = bits_new(nibble_len * 4);
	for (size_t i = 0; i < nibble_len; ++i) {
		char hex = hex_str[i];
		char nibble = 0;
		if ('0' <= hex && hex <= '9') {
			nibble = (char)(hex - '0');
		} else if ('a' <= hex && hex <= 'f') {
			nibble = (char)(hex - 'a' + 10);
		} else if ('A' <= hex && hex <= 'F') {
			nibble = (char)(hex - 'A' + 10);
		}
		result->bits[i / 2] |= (nibble << (1 - (i % 2)) * 4);
	}
	return result;
}

bits_t *bits_from_bin(const char *bin_str) {
	size_t bit_len = strlen(bin_str);
	bits_t *result = bits_new(bit_len);
	for (size_t i = 0; i < bit_len; ++i) {
		if (bin_str[i] == '1') {
			result->bits[i / 8] |= 1 << (7 - (i % 8));
		}
	}
	return result;
}

bits_t *bits_from_raw(const unsigned char *bits, size_t bit_len) {
	bits_t *result = try_calloc(sizeof(bits_t));
	result->bitlen = bit_len;
	result->allocated = BYTE_LEN(bit_len);
	if (bit_len > 0) result->bits = try_memdup(bits, result->allocated);
	return result;
}

bits_t *bits_from_bitvec(GEN v) {
	size_t bit_len = (size_t)glength(v);
	bits_t *result = bits_new(bit_len);
	for (size_t i = 0; i < bit_len; ++i) {
		if (gel(v, i + 1) == (GEN)1) result->bits[i / 8] |= 1 << (7 - (i % 8));
	}
	return result;
}

GEN bits_to_i(const bits_t *bits) {
	pari_sp ltop = avma;
	GEN result = stoi(0);
	for (size_t i = 0; i < bits->bitlen; ++i) {
		if (GET_BIT(bits->bits, i) != 0)
			result = addii(result, int2n(bits->bitlen - i - 1));
	}
	return gerepilecopy(ltop, result);
}

char *bits_to_hex(const bits_t *bits) {
	char *result = try_calloc(BYTE_LEN(bits->bitlen) * 2 + 1);
	// probably right pad with zeroes, as thats what is actually stored.
	for (size_t i = 0; i < BYTE_LEN(bits->bitlen); ++i) {
		sprintf(result + (i * 2), "%02x", bits->bits[i]);
	}
	return result;
}

char *bits_to_bin(const bits_t *bits) {
	char *result = try_calloc(bits->bitlen + 1);
	for (size_t i = 0; i < bits->bitlen; ++i) {
		sprintf(result + i, "%1u", GET_BIT(bits->bits, i));
	}
	return result;
}

unsigned char *bits_to_raw(const bits_t *bits) {
	if (bits->bitlen == 0) {
		return NULL;
	}
	return try_memdup(bits->bits, BYTE_LEN(bits->bitlen));
}

size_t bits_to_rawlen(const bits_t *bits) { return BYTE_LEN(bits->bitlen); }

GEN bits_to_bitvec(const bits_t *bits) {
	GEN bitvec = gtovecsmall0(gen_0, bits->bitlen);
	for (size_t i = 0; i < bits->bitlen; ++i) {
		if (GET_BIT(bits->bits, i) != 0) gel(bitvec, i + 1) = (GEN)1;
	}
	return bitvec;
}

static unsigned char or_func(unsigned char one, unsigned char other) {
	return one | other;
}

static unsigned char and_func(unsigned char one, unsigned char other) {
	return one & other;
}

static bits_t *bits_bitwise(const bits_t *one, const bits_t *other,
                            unsigned char (*bitwise_func)(unsigned char,
                                                          unsigned char)) {
	const bits_t *shorter;
	const bits_t *longer;
	if (one->bitlen > other->bitlen) {
		shorter = other;
		longer = one;
	} else {
		shorter = one;
		longer = other;
	}

	bits_t *result = bits_new(longer->bitlen);
	for (size_t i = 0; i < longer->bitlen; ++i) {
		size_t longer_pos = longer->bitlen - i - 1;

		unsigned char longer_bit =
		    (unsigned char)GET_BIT(longer->bits, longer_pos);
		unsigned char shorter_bit = 0;
		if (shorter->bitlen > i) {
			size_t shorter_pos = shorter->bitlen - i - 1;
			shorter_bit = (unsigned char)GET_BIT(shorter->bits, shorter_pos);
		}

		unsigned char result_bit = bitwise_func(longer_bit, shorter_bit);
		result->bits[longer_pos / 8] |= result_bit << (7 - (longer_pos % 8));
	}

	return result;
}

void bits_concatzv(bits_t *one, va_list valist) {
	const bits_t *next;
	while ((next = va_arg(valist, const bits_t *)) != NULL) {
		if (next->bitlen == 0) continue;
		size_t new_bitlen = one->bitlen + next->bitlen;
		size_t new_alloc = BYTE_LEN(new_bitlen);
		if (new_alloc > one->allocated) {
			one->bits = try_realloc(one->bits, new_alloc);
			for (size_t i = one->allocated; i < new_alloc; ++i) {
				one->bits[i] = 0;
			}
			one->allocated = new_alloc;
		}
		for (size_t j = 0; j < next->bitlen; ++j) {
			SET_BIT(one->bits, one->bitlen + j, GET_BIT(next->bits, j));
		}
		one->bitlen = new_bitlen;
	}
}

void bits_concatz(bits_t *one, ...) {
	va_list valist;
	va_start(valist, one);

	bits_concatzv(one, valist);

	va_end(valist);
}

bits_t *bits_concat(const bits_t *one, ...) {
	va_list valist;
	va_start(valist, one);

	bits_t *result = bits_copy(one);

	bits_concatzv(result, valist);

	va_end(valist);
	return result;
}

bits_t *bits_or(const bits_t *one, const bits_t *other) {
	return bits_bitwise(one, other, or_func);
}

bits_t *bits_and(const bits_t *one, const bits_t *other) {
	return bits_bitwise(one, other, and_func);
}

void bits_notz(bits_t *bits) {
	if (bits->bitlen == 0) return;
	for (size_t i = 0; i < bits->bitlen / 8; ++i) {
		bits->bits[i] = ~bits->bits[i];
	}
	if (bits->bitlen % 8 != 0) {
		size_t mask_len = bits->bitlen % 8;
		unsigned char mask = 0;
		for (size_t i = 7; i > 0; --i) {
			mask |= 1 << i;
			if (--mask_len == 0) break;
		}
		size_t last_pos = (bits->bitlen / 8);
		unsigned char anti_mask = ~mask;
		unsigned char last_byte = bits->bits[last_pos];
		bits->bits[last_pos] = (~last_byte & mask) | (last_byte & anti_mask);
	}
}

bits_t *bits_not(const bits_t *bits) {
	bits_t *result = bits_copy(bits);
	bits_notz(result);
	return result;
}

void bits_rotz(bits_t *bits) {
	unsigned char original_bits[bits->allocated];
	for (size_t i = 0; i < bits->allocated; ++i) {
		original_bits[i] = bits->bits[i];
		bits->bits[i] = 0;
	}
	for (size_t i = 0; i < bits->bitlen / 2; ++i) {
		size_t left_pos = i;
		size_t right_pos = bits->bitlen - i - 1;
		unsigned char left_bit =
		    (unsigned char)GET_BIT(original_bits, left_pos);
		unsigned char right_bit =
		    (unsigned char)GET_BIT(original_bits, right_pos);
		bits->bits[right_pos / 8] |= left_bit << (7 - (right_pos % 8));
		bits->bits[left_pos / 8] |= right_bit << (7 - (left_pos % 8));
	}
	if (bits->bitlen % 2 == 1) {
		size_t middle_pos = bits->bitlen / 2;

		unsigned char middle_bit =
		    (unsigned char)GET_BIT(original_bits, middle_pos);
		bits->bits[middle_pos / 8] |= middle_bit << (7 - (middle_pos % 8));
	}
}

bits_t *bits_rot(const bits_t *bits) {
	bits_t *result = bits_copy(bits);
	bits_rotz(result);
	return result;
}

void bits_shiftz(bits_t *bits, long amount) {
	if (amount == 0) return;
	unsigned char original_bits[bits->allocated];
	for (size_t i = 0; i < bits->allocated; ++i) {
		original_bits[i] = bits->bits[i];
		bits->bits[i] = 0;
	}
	for (size_t i = 0; i < bits->bitlen; ++i) {
		unsigned char new_bit = 0;
		if ((amount > 0 && i + amount < bits->bitlen) ||
		    (amount < 0 && i >= -amount)) {
			new_bit = (unsigned char)GET_BIT(original_bits, i + amount);
		}
		bits->bits[i / 8] |= new_bit << (7 - (i % 8));
	}
}

bits_t *bits_shift(const bits_t *bits, long amount) {
	bits_t *result = bits_copy(bits);
	bits_shiftz(result, amount);
	return result;
}

void bits_shiftrz(bits_t *bits, long amount) {
	if (amount == 0) return;
	unsigned char original_bits[bits->allocated];
	for (size_t i = 0; i < bits->allocated; ++i) {
		original_bits[i] = bits->bits[i];
		bits->bits[i] = 0;
	}
	for (size_t i = 0; i < bits->bitlen; ++i) {
		unsigned char new_bit = 0;
		size_t new_pos = 0;
		if ((amount > 0 && i + amount < bits->bitlen) ||
		    (amount < 0 && i >= -amount)) {
			new_pos = i + amount;
		} else if (amount > 0) {
			new_pos = (i + amount) % bits->bitlen;
		} else if (amount < 0) {
			long mod_amount = amount % bits->bitlen;
			new_pos = (i + mod_amount) % bits->bitlen;
		}
		new_bit = (unsigned char)GET_BIT(original_bits, new_pos);
		bits->bits[i / 8] |= new_bit << (7 - (i % 8));
	}
}

bits_t *bits_shiftr(const bits_t *bits, long amount) {
	bits_t *result = bits_copy(bits);
	bits_shiftrz(result, amount);
	return result;
}

void bits_shiftiz(bits_t *bits, long amount) {
	if (amount > 0) {
		bits_lengthenz(bits, -amount);
	} else if (amount < 0) {
		bits_shortenz(bits, amount);
	}
}

bits_t *bits_shifti(const bits_t *bits, long amount) {
	bits_t *result = bits_copy(bits);
	bits_shiftiz(result, amount);
	return result;
}

void bits_lengthenz(bits_t *bits, long amount) {
	size_t abs_amount;
	if (amount > 0) {
		abs_amount = (size_t)amount;
	} else if (amount < 0) {
		abs_amount = (size_t)-amount;
	} else {
		return;
	}
	size_t new_alloc = BYTE_LEN(bits->bitlen + abs_amount);
	if (new_alloc > bits->allocated) {
		bits->bits = try_realloc(bits->bits, new_alloc);
		for (size_t i = bits->allocated; i < new_alloc; ++i) {
			bits->bits[i] = 0;
		}
		bits->allocated = new_alloc;
	}
	bits->bitlen += abs_amount;

	if (amount > 0) {
		bits_shiftz(bits, -amount);
	}
}

bits_t *bits_lengthen(const bits_t *bits, long amount) {
	bits_t *result = bits_copy(bits);
	bits_lengthenz(result, amount);
	return result;
}

void bits_shortenz(bits_t *bits, long amount) {
	size_t new_bits;
	if (amount > 0) {
		new_bits = bits->bitlen - amount;
		bits_shiftz(bits, amount);
	} else if (amount < 0) {
		new_bits = bits->bitlen + amount;
		for (size_t i = new_bits; i < bits->bitlen; ++i) {
			bits->bits[i / 8] &= ~(1 << (7 - (i % 8)));
		}
	} else {
		return;
	}
	bits->bitlen = new_bits;
}

bits_t *bits_shorten(const bits_t *bits, long amount) {
	bits_t *result = bits_copy(bits);
	bits_shortenz(result, amount);
	return result;
}

void bits_sha1(const bits_t *bits, unsigned char hashout[20]) {
	SHA_CTX ctx = {0};
	SHA1_Init(&ctx);
	SHA1_Update(&ctx, bits->bits, (int)BYTE_LEN(bits->bitlen));
	SHA1_Final(hashout, &ctx);
}

bool bits_eq(const bits_t *one, const bits_t *other) {
	if (one->bitlen != other->bitlen) return false;
	if (one->bitlen == 0) return true;
	if (memcmp(one->bits, other->bits, one->bitlen / 8) != 0) return false;
	if (one->bitlen % 8 != 0) {
		size_t mask_len = one->bitlen % 8;
		unsigned char mask = 0;
		for (size_t i = 7; i > 0; --i) {
			mask |= 1 << i;
			if (--mask_len == 0) break;
		}
		size_t last_byte = (one->bitlen / 8);
		unsigned char one_masked = one->bits[last_byte] & mask;
		unsigned char other_masked = other->bits[last_byte] & mask;
		return one_masked == other_masked;
	}
	return true;
}
