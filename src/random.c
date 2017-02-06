/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "random.h"

GEN random_prime(long bits) {
	pari_sp ltop = avma;

	GEN range = gtovec0(gen_0, 2);
	gel(range, 1) = powis(gen_2, bits - 1);
	gel(range, 2) = powis(gen_2, bits);

	GEN p;
	{
		pari_sp btop = avma;
		do {
			p = randomprime(range);
			if (gc_needed(btop, 1)) p = gerepilecopy(btop, p);
		} while (!isprime(p));
	}

	return gerepilecopy(ltop, p);
}

GEN random_int(long bits) {
	pari_sp ltop = avma;

	GEN range = gtovec0(gen_0, 2);
	gel(range, 1) = powis(gen_2, bits - 1);
	gel(range, 2) = powis(gen_2, bits);

	return gerepilecopy(ltop, genrand(range));
}