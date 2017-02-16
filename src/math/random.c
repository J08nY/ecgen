/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#define _POSIX_C_SOURCE 200809L
#include "random.h"
#include <time.h>

bool random_init(void) {
	pari_ulong seed = 0;
	// Try urandom first
	FILE *rand = fopen("/dev/urandom", "rb");
	if (rand) {
		fread(&seed, 1, sizeof(pari_ulong), rand);
		fclose(rand);
	}
	// Try worse methods later
	if (seed == 0) {
		struct timespec t;
		if (!clock_gettime(CLOCK_REALTIME, &t)) {
			seed = (pari_ulong)t.tv_nsec;
		} else {
			seed = (pari_ulong)time(NULL);
		}
	}

	pari_sp ltop = avma;
	setrand(utoi(seed));
	avma = ltop;

	return true;
}

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
