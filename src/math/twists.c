/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "twists.h"
#include "gen/seed.h"
#include "obj/subgroup.h"

void twist_rand_to(curve_t *to, const curve_t *of) {
	to->field = gcopy(of->field);
	GEN v = elltwist(of->curve, NULL);
	to->curve = ellinit(v, to->field, -1);
	if (typ(of->field) == t_INT) {
		to->a = ell_get_a4(to->curve);
		to->b = ell_get_a6(to->curve);
		if (of->order != NULL) {
			GEN q = addis(mulis(of->field, 2), 2);
			to->order = subii(q, of->order);
			obj_insert_shallow(to->curve, 1, to->order);
		}
	} else if (typ(of->field) == t_FFELT) {
		to->a = ell_get_a2(to->curve);
		to->b = ell_get_a6(to->curve);
		if (of->order != NULL) {
			GEN q = addis(mulis(int2n(degree(FF_mod(of->field))), 2), 2);
			to->order = subii(q, of->order);
			obj_insert_shallow(to->curve, 1, to->order);
		}
	}
}

void twist_rand(curve_t *what) {
	twist_rand_to(what, what);
	seed_free(&what->seed);
	subgroups_free_deep(&what->generators, what->ngens);
}