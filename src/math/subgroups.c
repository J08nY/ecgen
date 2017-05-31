/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "subgroups.h"

GEN subgroups_prime(GEN order, const config_t *cfg) {
	if (cfg->prime || isprime(order)) {
		return gtovec(order);
	} else {
		GEN factors = Z_factor(order);
		return gel(factors, 1);
	}
}

// TODO: what if some factor multiple times? Prove this works..
static GEN subgroups_2n(GEN factors, size_t min_bits) {
	long nprimes = glength(factors);
	GEN amount = int2n(nprimes);
	GEN groups = gtovec0(gen_0, itos(amount) - (min_bits * nprimes) - 1);

	size_t i = 0;
	for (size_t count = 1; count < (size_t)(1 << nprimes); ++count) {
		pari_sp btop = avma;
		GEN result = gen_1;
		size_t bits = 0;
		for (long bit = 0; bit < nprimes; ++bit) {
			size_t mask = (size_t)(1 << bit);
			if (count & mask) {
				result = mulii(result, gel(factors, bit + 1));
				bits++;
			}
		}
		if (bits > min_bits) {
			gel(groups, ++i) = result;
		} else {
			avma = btop;
		}
	}
	// TODO: sort this, as it is not sorted
	return groups;
}

GEN subgroups_nonprime(GEN order, const config_t *cfg) {
	if (cfg->prime || isprime(order)) {
		return NULL;
	} else {
		GEN factors = subgroups_prime(order, cfg);
		return subgroups_2n(factors, 1);
	}
}

GEN subgroups_all(GEN order, const config_t *cfg) {
	if (cfg->prime || isprime(order)) {
		return gtovec(order);
	} else {
		GEN factors = subgroups_prime(order, cfg);
		return subgroups_2n(factors, 0);
	}
}