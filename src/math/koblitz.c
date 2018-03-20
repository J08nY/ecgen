/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "koblitz.h"
#include "gen/field.h"

/**
 * Data from:
 *  Guide to Elliptic Curve Cryptography,
 *  Darrel Hankerson, Alfred Menezes, Scott Vanstone
 *  Springer, 2004
 */
static koblitz_t koblitz_curves[] = {
    // clang-format off
		{101, 1, "0x2000000000000a8c8e37728cd6"},
		{103, 0, "0x8000000000000a6a298f2129f4"},
		{107, 0, "0x7ffffffffffffb57c25324737c4"},
		{107, 1, "0x80000000000004a83dacdb8c83e"},
		{109, 1, "0x1fffffffffffff4c436058707d36"},
		{113, 1, "0x1fffffffffffffffb7f235edbd4e6"},
		{131, 0, "0x80000000000000001353f755c0e8fc9a4"},
		{163, 1, "0x800000000000000000004021145c1981b33f14bde"},
		{233, 0, "0x200000000000000000000000000001a756ee456f351bbec6b57c5ceaf7c"},
		{239, 0, "0x80000000000000000000000000000169e7fb19f2dba47c7076a00391e294"},
		{277, 0, "0x1ffffffffffffffffffffffffffffffffffb42a2d15e3f4d2f69828d921e5bb03c3eec"},
		{283, 0, "0x7ffffffffffffffffffffffffffffffffffa6b8bb41d5dc9977fdfe511478187858f184"},
		{283, 1, "0x80000000000000000000000000000000000594744be2a2366880201aeeb87e787a70e7e"},
		{311, 1, "0x7ffffffffffffffffffffffffffffffffffffff136b216132e5082acf66a36f8f226eb6b04ddce"},
		{331, 1, "0x8000000000000000000000000000000000000000014cf64d52882df2e59be7970195b73750ef4048ebe"},
		{347, 1, "0x7ffffffffffffffffffffffffffffffffffffffffffe7eb05ecfb792870172fe61a4926fc197f7b6801597e"},
		{349, 0, "0x200000000000000000000000000000000000000000004f4b4f74ea048634aaca843ad4252e1d28422edd038c"},
		{359, 1, "0x7fffffffffffffffffffffffffffffffffffffffffffedec280d2e16075c01f6a24e2d482a3941d593a361c20e"},
		{409, 0, "0x1fffffffffffffffffffffffffffffffffffffffffffffffffff97e0ecb53a881003b1155f57b4f8f9f296d2d720ee380797f3c"},
		{571, 0, "0x800000000000000000000000000000000000000000000000000000000000000000000004c614387c6698f92ce46a36e45fd04e2d8c3612f9758e4e07a477ad173f9de3d8df04004"}
    // clang-format on
};

bool koblitz_is_curve(const curve_t *curve) {
	pari_sp ltop = avma;
	GEN a = field_elementi(curve->a);
	GEN b = field_elementi(curve->b);
	bool result = (gequal(a, gen_0) || gequal(a, gen_1)) && gequal(b, gen_1);
	avma = ltop;
	return result;
}

static int compare_koblitz(const void *a, const void *b) {
	const koblitz_t *one = a;
	const koblitz_t *other = b;
	int m_diff = one->m - other->m;
	if (m_diff == 0) {
		return one->a - other->a;
	} else {
		return m_diff;
	}
}

const koblitz_t *koblitz_find(unsigned long m, unsigned long a) {
	koblitz_t searched = {(unsigned int)m, (unsigned int)a, NULL};
	return (koblitz_t *)bsearch(&searched, koblitz_curves,
	                            sizeof(koblitz_curves) / sizeof(koblitz_t),
	                            sizeof(koblitz_t), &compare_koblitz);
}

GEN koblitz_get_order(unsigned long m, unsigned long a) {
	const koblitz_t *found = koblitz_find(m, a);
	if (found) {
		return strtoi(found->hex_order);
	} else {
		return NULL;
	}
}