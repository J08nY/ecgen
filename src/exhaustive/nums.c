/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2023 J08nY
 */

#include "nums.h"
#include "math/subgroup.h"
#include "obj/point.h"
#include "obj/subgroup.h"

GENERATOR(nums_gen_field) {
	pari_sp ltop = avma;
	GEN base = int2u(cfg->bits);
	GEN c = stoi(1);
	GEN p = subii(base, c);
	while (!isprime(p)) {
		c = addiu(c, 4);
		p = subii(base, c);
	}
	curve->field = gerepilecopy(ltop, p);
	return 1;
}

GENERATOR(nums_gen_a) {
	curve-> a = gmodulo(stoi(-3), curve->field);
	return 1;
}

static GEN b = NULL;
static curve_t *curve_b = NULL;

GENERATOR(nums_gen_b) {
	pari_sp ltop = avma;
	if (!b) {
		b = gclone(gen_1);
		curve->b = gmodulo(gen_1, curve->field);
		curve_b = curve;
		return 1;
	} else {
		if (curve_b == curve) {
			// use b, add 1 to it, store it back modified,
			GEN bn;
			if (equaliu(b, 1)) {
				bn = addiu(b, 2);
			} else {
				bn = addiu(b, 1);
			}
			gunclone(b);
			b = gclone(bn);
			curve->b = gerepilecopy(ltop, gmodulo(bn, curve->field));
			return 1;
		} else {
			// dont use b, regenerate it,
			gunclone(b);
			b = gclone(gen_1);
			curve->b = gerepilecopy(ltop, gmodulo(gen_1, curve->field));
			curve_b = curve;
			return 1;
		}
	}
}

GENERATOR(nums_gen_order) {
	pari_sp ltop = avma;
	GEN order = ellsea(curve->curve, 1);
	GEN frobenius = subii(addis(curve->field, 1), order);
	GEN twist_order = addii(order, muliu(frobenius, 2));
	if (gequal0(order) || !(isprime(order)) || !(isprime(twist_order))) {
		avma = ltop;
		return -2;  // Just go one step back and regenerate the b.
	} else {
		curve->order = gerepilecopy(ltop, order);
		obj_insert_shallow(curve->curve, 1, curve->order);
		return 1;
	}
}

GENERATOR(nums_gen_gens) {
	pari_sp ltop = avma;
	GEN x = stoi(1);
	GEN Qy = ellordinate(curve->curve, x, 0);
	while (glength(Qy) == 0) {
		x = addiu(x, 1);
		Qy = ellordinate(curve->curve, x, 0);
	}
	GEN P = NULL;
	if (cmpii(lift(gel(Qy, 1)), lift(gel(Qy, 2))) < 0) {
		P = mkvec2(gmodulo(x, curve->field), gel(Qy, 1));
	} else {
		P = mkvec2(gmodulo(x, curve->field), gel(Qy, 2));
	}

	curve->generators = subgroups_new(1);
	curve->ngens = 1;
	subgroup_t *sub = subgroup_new();
	curve->generators[0] = sub;
	point_t *G = point_new();
	sub->generator = G;
	G->point = P;
	G->order = ellorder(curve->curve, G->point, NULL);
	G->cofactor = divii(curve->order, G->order);
	gerepileall(ltop, 3, &G->point, &G->order, &G->cofactor);
	return 1;
}


void nums_quit(void) {
	if (b && isclone(b)) {
		gunclone(b);
	}
}