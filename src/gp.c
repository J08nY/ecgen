#include "gp.h"

void init_gp(void) /* void */
{
	pari_sp ltop = avma;
	avma = ltop;
	return;
}

/*
* ecgen, tool for generating Elliptic curve domain parameters
* Copyright (C) 2017  J08nY
*/

/*
\r gp/utils
\r gp/invalid
*/

GEN test(void) /* int */
{
	pari_sp ltop = avma;
	GEN e = gen_0; /* ell */
	GEN p = gen_0;
	GEN i = gen_0, p1 = gen_0; /* int */
	p = genrand(e);
	p1 = i = gcopy(gel(p, 1));
	p1 = gerepilecopy(ltop, p1);
	return p1;
}
