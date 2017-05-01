/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file cm.c
 */
#include "cm.h"
#include "exhaustive/anomalous.h"
#include "p1363.h"

int cm_do(config_t *cfg) {
	GEN D = stoi(71);
	form_t **forms;
	size_t nforms = p1363_forms(D, &forms);
	for (size_t i = 0; i < nforms; ++i) {
		p1363_invariant(D, forms[i]);
	}

	p1363_free(&forms, nforms);
	return 0;
}
