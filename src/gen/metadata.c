/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "gens.h"
#include "field.h"
#include "metadata.h"

GENERATOR(metadata_gen) {
	GEN j = field_elementi(ell_get_j(curve->curve));
	GEN disc = field_elementi(ell_get_disc(curve->curve));
	GEN embedding_degree;
	GEN q;
	if (typ(curve->field) == t_INT) {
		embedding_degree = gens_get_embedding(curve->field, curve->order);
		q = curve->field;
	} else if (typ(curve->field) == t_FFELT){
		embedding_degree = NULL;
		q = int2n(degree(FF_mod(curve->field)));
	} else {
		return -7;
	}
	GEN frobenius = negi(subii(curve->order, addis(q, 1)));
	GEN cm_disc;
	if (typ(curve->field) == t_INT) {
		cm_disc = core(subii(mulis(q, 4), sqri(frobenius)));
	} else if (typ(curve->field) == t_FFELT){
		cm_disc = NULL;
	} else {
		return -7;
	}
	curve->meta.j_invariant = j;
	curve->meta.discriminant = disc;
	curve->meta.embedding_degree = embedding_degree;
	curve->meta.frobenius_trace = frobenius;
	curve->meta.cm_discriminant = cm_disc;
	return 1;
}
