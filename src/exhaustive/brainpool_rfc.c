/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */

#include "brainpool_rfc.h"
#include "brainpool.h"
#include "util/bits.h"

#define brainpool_delegate(func)            \
	int ret = func(curve, args, state);     \
	if (ret != 1) {                         \
		return ret;                         \
	}                                       \
	curve->seed->type = SEED_BRAINPOOL_RFC; \
	return 1;

GENERATOR(brainpool_rfc_gen_seed_argument) {
	brainpool_delegate(brainpool_gen_seed_argument);
}

GENERATOR(brainpool_rfc_gen_seed_random) {
	brainpool_delegate(brainpool_gen_seed_random);
}

GENERATOR(brainpool_rfc_gen_seed_input) {
	brainpool_delegate(brainpool_gen_seed_input);
}

#undef brainpool_delegate

GENERATOR(brainpool_rfc_gen_equation) {
	// field is definitely prime
	pari_sp btop = avma;
	seed_t *seed = curve->seed;
	do {
		if (seed->brainpool.update_seed) {
			brainpool_update_seed(seed->seed);
			seed->brainpool.update_seed = false;
		}

		bits_t *a_bits =
		    brainpool_hash(seed->seed, seed->brainpool.w, seed->brainpool.v);
		GEN a = bits_to_i(a_bits);
		bits_free(&a_bits);
		GEN am = Fp_invsafe(a, curve->field);
		if (am == NULL) {
			brainpool_update_seed(seed->seed);
			avma = btop;
			continue;
		}
		GEN z;
		z = Fp_sqrtn(Fp_muls(am, -3, curve->field), stoi(4), curve->field,
		             NULL);
		if (z == NULL) {
			brainpool_update_seed(seed->seed);
			avma = btop;
			continue;
		}
		// seed->brainpool.seed_a = bits_copy(seed->seed);

		GEN b = NULL;
		pari_sp bbtop = avma;
		do {
			if (b != NULL) {
				avma = bbtop;
			}
			brainpool_update_seed(seed->seed);
			bits_t *b_bits = brainpool_hash(seed->seed, seed->brainpool.w,
			                                seed->brainpool.v);
			b = bits_to_i(b_bits);
			bits_free(&b_bits);
		} while (Fp_issquare(b, curve->field));

		// seed->brainpool.seed_b = bits_copy(seed->seed);

		GEN mod_a = gmodulo(a, curve->field);
		GEN mod_b = gmodulo(b, curve->field);

		if (gequal0(gmulsg(-16, gadd(gmulsg(4, gpowgs(mod_a, 3)),
		                             gmulsg(27, gsqr(mod_b)))))) {
			brainpool_update_seed(seed->seed);
			// bits_free(&seed->brainpool.seed_a);
			// bits_free(&seed->brainpool.seed_b);
			avma = btop;
			continue;
		}

		curve->a = mod_a;
		curve->b = mod_b;
		gerepileall(btop, 2, &curve->a, &curve->b);
		break;
	} while (true);

	seed->brainpool.update_seed = true;
	return 1;
}
