/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "metadata.h"
#include "field.h"
#include "gens.h"

GENERATOR(metadata_gen) {
	pari_sp ltop = avma;
	GEN j = field_elementi(ell_get_j(curve->curve));
	GEN disc = field_elementi(ell_get_disc(curve->curve));
	GEN embedding_degree;
	GEN q;
	if (typ(curve->field) == t_INT) {
		embedding_degree = gens_get_embedding(curve->field, curve->order);
		q = curve->field;
	} else if (typ(curve->field) == t_FFELT) {
		embedding_degree = NULL;
		q = int2n(degree(FF_mod(curve->field)));
	} else {
		return -7;
	}
	GEN frobenius = subii(addis(q, 1), curve->order);
	GEN conductor;
	GEN cm_disc;
	if (typ(curve->field) == t_INT) {
		GEN d_f = coredisc2(subii(sqri(frobenius), mulis(q, 4)));
		cm_disc = gcopy(gel(d_f, 1));
		conductor = gcopy(gel(d_f, 2));
		gerepileall(ltop, 6, &j, &disc, &embedding_degree, &frobenius, &cm_disc, &conductor);
	} else if (typ(curve->field) == t_FFELT) {
		cm_disc = NULL;
		conductor = NULL;
		gerepileall(ltop, 3, &j, &disc, &frobenius);
	} else {
		return -7;
	}
	curve->meta.j_invariant = j;
	curve->meta.discriminant = disc;
	curve->meta.embedding_degree = embedding_degree;
	curve->meta.frobenius_trace = frobenius;
	curve->meta.cm_discriminant = cm_disc;
	curve->meta.conductor = conductor;
	return 1;
}
