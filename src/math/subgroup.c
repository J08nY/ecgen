/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "subgroup.h"

/**
 * @brief All prime divisors of a given integer with multiplicity.
 *
 * subgroups_divisors(27) = [3, 3, 3]
 * @param order
 * @return a t_VEC of prime divisors.
 */
static GEN subgroups_divisors(GEN order) {
	GEN factors = Z_factor(order);
	GEN primes = gel(factors, 1);
	GEN multiples = gel(factors, 2);
	long uniqs = glength(primes);

	long size = 0;
	for (long i = 1; i <= uniqs; ++i) {
		size += itos(gel(multiples, i));
	}
	GEN result = gtovec0(gen_0, size);

	long count = 0;
	for (long i = 1; i <= uniqs; ++i) {
		long multiple = itos(gel(multiples, i));
		for (long j = 1; j <= multiple; ++j) {
			gel(result, ++count) = gel(primes, i);
		}
	}
	return result;
}

/**
 * @brief All factors consisting of at least <code>min_bits</code> prime
 * <code>factors</code>.
 *
 * @param factors
 * @param min_bits
 * @return a t_VEC of factors
 */
static GEN subgroups_2n_factors(GEN factors, size_t min_bits) {
	pari_sp ltop = avma;
	long nprimes = glength(factors);
	if (nprimes == min_bits) return NULL;
	GEN amount = int2n(nprimes);
	GEN groups = gtovec0(gen_0, itos(amount) - (min_bits * nprimes) - 1);

	size_t i = 0;
	for (size_t count = 1; count < (size_t)(1) << nprimes; ++count) {
		pari_sp btop = avma;
		GEN result = gen_1;
		size_t bits = 0;
		for (long bit = 0; bit < nprimes; ++bit) {
			size_t mask = (size_t)(1) << bit;
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
	GEN ret = gtoset(groups);
	return gerepilecopy(ltop, ret);
}

GEN subgroups_prime(GEN order) {
	if (isprime(order)) {
		return gtovec(order);
	}
	GEN factors = Z_factor(order);
	return gtovec(gel(factors, 1));
}

GEN subgroups_nonprime(GEN order) {
	if (isprime(order)) {
		return NULL;
	}
	GEN factors = subgroups_divisors(order);
	return subgroups_2n_factors(factors, 1);
}

GEN subgroups_all(GEN order) {
	if (isprime(order)) {
		return gtovec(order);
	}
	GEN factors = subgroups_divisors(order);
	return subgroups_2n_factors(factors, 0);
}

/**
 * @brief
 * @param curve
 * @param min_bits
 * @return
 */
/*
static GEN subgroups_2n_gens(const curve_t *curve, size_t min_bits) {
    GEN one_factors = subgroups_divisors(curve->generators[0]->order);
    GEN one = subgroups_2n_factors(one_factors, min_bits);
    GEN other_factors = subgroups_divisors(curve->generators[1]->order);
    GEN other = subgroups_2n_factors(other_factors, min_bits);
    if (!one) {
        return other;
    }
    if (!other) {
        return one;
    }
    GEN result = gtovec0(gen_0, glength(one) + glength(other));
    for (long i = 1; i <= glength(result); ++i) {
        if (i <= glength(one)) {
            gel(result, i) = gel(one, i);
        } else {
            gel(result, i) = gel(other, i - glength(one));
        }
    }
    return result;
}
 */

/**
 * @brief
 * @param curve
 * @param min_bits
 * @return
 */
/*
static GEN subgroups_2n(const curve_t *curve, size_t min_bits) {
   if (curve->ngens == 1) {
       GEN factors = subgroups_divisors(curve->order);
       return subgroups_2n_factors(factors, min_bits);
   }

   return subgroups_2n_gens(curve, min_bits);
}

*/