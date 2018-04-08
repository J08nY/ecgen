/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file p1363.h
 */
#ifndef ECGEN_CM_P1363_H
#define ECGEN_CM_P1363_H

#include <pari/pari.h>

typedef struct {
	GEN A;
	GEN B;
	GEN C;

	long m8;

	long I;
	long J;
	long K;
	GEN L;
	GEN M;
	GEN N;

	GEN lambda;
	GEN theta;
} p1363_form_t;

/**
 * Compute all the primitive reduced quadratic forms for a given discriminant.
 * @param D
 * @param forms
 * @return
 */
size_t p1363_forms(GEN D, p1363_form_t ***forms);

/**
 * Free the computed quadratic forms.
 * @param forms
 * @param nforms
 */
void p1363_free(p1363_form_t ***forms, size_t nforms);

/**
 * Compute the class invariant for discriminant D and form.
 * @param D
 * @param form
 * @param precision
 * @return
 */
GEN p1363_invariant(GEN D, p1363_form_t *form, long precision);

/**
 * Bit-precision computation for a Weber class polynomial from:
 * 		On the Efficient Generation of Elliptic Curves,
 * 		Elisavet Konstantinou, Yiannis C. Stamatiou, Christos Zaroliagis
 * @param D
 * @param forms
 * @param nforms
 * @return The pari precision required for W_D.
 */
long p1363_bit_precision(GEN D, p1363_form_t **forms, size_t nforms);

/**
 * Compute the reduced Webe class polynomial for discriminant D and quadratic
 * forms.
 * @param D
 * @param forms
 * @param nforms
 * @return
 */
GEN p1363_poly(GEN D, p1363_form_t **forms, size_t nforms);

GEN p1363_polclass(GEN D);

#endif  // ECGEN_CM_P1363_H
