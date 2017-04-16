/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file anomalous.c
 */
#include "anomalous.h"
#include "math/random.h"

static disc_t **disc_table;

void anomalous_init() {
	disc_table = pari_malloc(sizeof(disc_t *) * 5);
	if (!disc_table) {
		perror("Couldn't malloc.");
		exit(1);
	}
	for (int i = 0; i < 5; ++i) {
		disc_table[i] = pari_malloc(sizeof(disc_t));
		if (!disc_table[i]) {
			perror("Couldn't malloc.");
			exit(1);
		}
	}

	/*
	 * Discriminant, j-invariant and field-element (Alpha) from Atsuko Miyaji's
	 * paper.
	 */
	GEN a = stoi(-32);
	GEN b = stoi(-64);
	disc_table[0]->D = stoi(11);
	disc_table[0]->j = powis(a, 3);
	disc_table[0]->a = stoi(21);
	disc_table[1]->D = stoi(19);
	disc_table[1]->j = powis(mulis(a, 3), 3);
	disc_table[1]->a = stoi(3);
	disc_table[2]->D = stoi(43);
	disc_table[2]->j = powis(mulis(b, 16), 3);
	disc_table[2]->a = stoi(70);
	disc_table[3]->D = stoi(67);
	disc_table[3]->j = powis(mulis(a, 165), 3);
	disc_table[3]->a = stoi(35805);
	disc_table[3]->D = stoi(163);
	disc_table[3]->j = powis(mulis(b, 10005), 3);
	disc_table[3]->a = stoi(3717878010);
}

static GEN anomalous_prime(GEN D, const config_t *cfg) {
	pari_sp ltop = avma;
	GEN last = divis(addis(D, 1), 4);
	GEN middle;
	GEN first;
	GEN p;
	GEN b;

	do {
		b = random_int(cfg->bits);
		middle = mulii(D, b);
		first = mulii(middle, b);
		p = addii(addii(first, middle), last);
		if (gc_needed(ltop, 1)) {
			gerepileall(ltop, 2, &p, &last);
		}
	} while (!isprime(p));

	// TODO: this does not give me a bits sized prime..
	// but quite a larger one
	return gerepilecopy(ltop, p);
}

int anomalous_field(curve_t *curve, const config_t *cfg, arg_t *args) {
	// find suitable prime field
	curve->field = anomalous_prime(disc_table[0]->D, cfg);
	return 1;
}

int anomalous_equation(curve_t *curve, const config_t *cfg, arg_t *args) {
	// 1 on success, -2 on failure
	pari_sp ltop = avma;
	return INT_MIN;  // TODO this
}

int anomalous_order(curve_t *curve, const config_t *cfg, arg_t *args) {
	// copy field to order
	curve->order = gcopy(curve->field);
	return 1;
}

void anomalous_quit() {
	if (disc_table) {
		for (int i = 0; i < 5; ++i) {
			if (disc_table[i]) {
				pari_free(disc_table[i]);
			}
		}
		pari_free(disc_table);
	}
}
