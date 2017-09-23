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

size_t p1363_forms(GEN D, p1363_form_t ***forms);

void p1363_free(p1363_form_t ***forms, size_t nforms);

GEN p1363_invariant(GEN D, p1363_form_t *form);

GEN p1363_poly(GEN D, p1363_form_t **forms, size_t nforms);

#endif  // ECGEN_CM_P1363_H
