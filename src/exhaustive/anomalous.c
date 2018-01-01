/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "anomalous.h"
#include "io/output.h"
#include "util/memory.h"

static disc_t **disc_table;

void anomalous_init() {
	disc_table = try_calloc(sizeof(disc_t *) * 5);
	for (int i = 0; i < 5; ++i) {
		disc_table[i] = try_calloc(sizeof(disc_t));
	}

	/*
	 * Discriminant, j-invariant and field-element (Alpha) from Atsuko Miyaji's
	 * paper.
	 */
	GEN a = stoi(-32);
	GEN b = stoi(-64);
	disc_table[0]->d = stoi(11);
	disc_table[0]->j = powis(a, 3);
	disc_table[0]->alpha = stoi(21);
	disc_table[1]->d = stoi(19);
	disc_table[1]->j = powis(mulis(a, 3), 3);
	disc_table[1]->alpha = stoi(3);
	disc_table[2]->d = stoi(43);
	disc_table[2]->j = powis(mulis(b, 16), 3);
	disc_table[2]->alpha = stoi(70);
	disc_table[3]->d = stoi(67);
	disc_table[3]->j = powis(mulis(a, 165), 3);
	disc_table[3]->alpha = stoi(35805);
	disc_table[3]->d = stoi(163);
	disc_table[3]->j = powis(mulis(b, 10005), 3);
	disc_table[3]->alpha = stoi(3717878010);
}

static GEN anomalous_prime(size_t i, unsigned long bits) {
	GEN D = disc_table[i]->d;

	pari_sp ltop = avma;
	GEN last = divis(addis(D, 1), 4);
	GEN lower = divii(subii(int2n(bits - 1), last), D);
	GEN upper = divii(subii(int2n(bits), last), D);

	GEN lower_bound = gceil(
	    gdiv(gsub(gsqrt(addis(mulis(lower, 4), 1), BIGDEFAULTPREC * 2), gen_1),
	         gen_2));
	GEN upper_bound = gfloor(
	    gdiv(gsub(gsqrt(addis(mulis(upper, 4), 1), BIGDEFAULTPREC * 2), gen_1),
	         gen_2));

	GEN range = gtovec0(gen_0, 2);
	gel(range, 1) = lower_bound;
	gel(range, 2) = upper_bound;
	GEN first, middle;
	GEN p, b;

	pari_sp btop = avma;
	do {
		b = genrand(range);
		middle = mulii(D, b);
		first = mulii(middle, b);
		p = addii(addii(first, middle), last);
		gerepileall(btop, 2, &p, &last);
	} while (!isprime(p));

	return gerepilecopy(ltop, p);
}

static GEN anomalous_c(size_t i, GEN p) {
	pari_sp ltop = avma;
	long kroneck = kronecker(disc_table[i]->alpha, p);
	if (!kroneck) {
		return NULL;
	}
	long cp = -kroneck;

	GEN c;
	pari_sp btop = avma;
	do {
		c = genrand(p);
		gerepileall(btop, 1, &c);
	} while (kronecker(c, p) != cp);

	return gerepilecopy(ltop, c);
}

GENERATOR(anomalous_gen_field) {
	HAS_ARG(args);
	size_t i = *(size_t *)args->args;

	// find suitable prime field
	pari_sp ltop = avma;
	GEN p;
	do {
		p = anomalous_prime(i, cfg->bits);
		gerepileall(ltop, 1, &p);
	} while (!kronecker(disc_table[0]->alpha, p));

	curve->field = gerepilecopy(ltop, p);
	return 1;
}

GENERATOR(anomalous_gen_equation) {
	HAS_ARG(args);
	size_t i = *(size_t *)args->args;

	pari_sp ltop = avma;
	GEN c = anomalous_c(i, curve->field);

	GEN a_upper = gmodulo(disc_table[i]->j, curve->field);
	GEN a_lower = gmodulo(subsi(1728, disc_table[i]->j), curve->field);
	GEN a = gdiv(a_upper, a_lower);

	curve->a = gmul(stoi(3), gmul(gsqr(c), a));
	curve->b = gmul(gen_2, gmul(gpowgs(c, 3), a));
	gerepileall(ltop, 2, &curve->a, &curve->b);
	return 1;
}

GENERATOR(anomalous_gen_order) {
	// copy field to order
	curve->order = gcopy(curve->field);
	obj_insert(curve->curve, 1, curve->order);
	return 1;
}

void anomalous_quit() {
	if (disc_table) {
		for (int i = 0; i < 5; ++i) {
			if (disc_table[i]) {
				try_free(disc_table[i]);
			}
		}
		try_free(disc_table);
	}
}
