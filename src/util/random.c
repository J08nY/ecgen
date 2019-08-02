/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#define _POSIX_C_SOURCE 200809L

#include "random.h"
#include <time.h>
#include <stdint.h>

void random_reseed(void) {
	pari_ulong seed = 0;
	// Try urandom first
	FILE *rand = fopen("/dev/urandom", "rb");
	if (rand) {
		size_t read = 0;
		while (read < sizeof(pari_ulong)) {
			read += fread(((uint8_t*) &seed) + read, 1, sizeof(pari_ulong) - read, rand);
		}

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
}

bool random_init(void) {
	random_reseed();

	return true;
}

GEN random_prime(unsigned long bits) {
	pari_sp ltop = avma;

	GEN range = mkvec2(int2n(bits - 1), int2n(bits));

	GEN p;
	pari_sp btop = avma;
	do {
		p = randomprime(range);
		p = gerepileupto(btop, p);
	} while (!isprime(p));

	return gerepilecopy(ltop, p);
}

GEN random_int(unsigned long bits) {
	pari_sp ltop = avma;

	GEN range = mkvec2(int2n(bits - 1), int2n(bits));
	return gerepilecopy(ltop, genrand(range));
}

GEN random_range(GEN lower, GEN upper) {
	pari_sp ltop = avma;
	if (gequal(lower, upper)) {
		return gcopy(lower);
	}

	GEN range = mkvec2(lower, subis(upper, 1));
	return gerepilecopy(ltop, genrand(range));
}

GEN random_field_element(GEN field) {
	switch (typ(field)) {
		case t_INT:
			return mkintmod(genrand(field), field);
		case t_FFELT:
			return genrand(field);
		default:
			pari_err_TYPE("", field);
			return NULL;
	}
}
