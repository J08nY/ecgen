/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */

#include "brainpool.h"
#include "gen/gens.h"
#include "gen/seed.h"
#include "io/output.h"
#include "math/subgroup.h"
#include "obj/point.h"
#include "obj/subgroup.h"
#include "util/bits.h"
#include "util/str.h"

static seed_t *brainpool_new() {
	seed_t *result = seed_new();
	result->type = SEED_BRAINPOOL;
	return result;
}

static void seed_wv(seed_t *seed) {
	pari_sp ltop = avma;
	GEN L = utoi(cfg->bits);
	seed->brainpool.v = itou(gfloor(gdivgs(subis(L, 1), 160)));
	seed->brainpool.w = itou(subis(subis(L, 160 * seed->brainpool.v), 1));
	avma = ltop;
}

void brainpool_update_seed(bits_t *s) {
	pari_sp ltop = avma;
	GEN z = bits_to_i(s);
	GEN t = Fp_add(z, gen_1, int2n(160));
	bits_t *result = bits_from_i_len(t, 160);
	avma = ltop;
	bits_cpy(s, result);
	bits_free(&result);
}

bits_t *brainpool_hash(const bits_t *s, long w, long v) {
	pari_sp ltop = avma;
	unsigned char h[20];
	bits_sha1(s, h);
	unsigned char hashout[20 * v];

	GEN z = bits_to_i(s);
	GEN m = int2n(160);
	for (long i = 1; i <= v; ++i) {
		bits_t *si = bits_from_i_len(Fp_add(z, stoi(i), m), 160);
		bits_sha1(si, hashout + (20 * (i - 1)));
		bits_free(&si);
	}
	bits_t *result = bits_from_raw(h, 20 * 8);
	bits_shortenz(result, 20 * 8 - w);
	bits_t *rest = bits_from_raw(hashout, (size_t)(20 * v * 8));
	bits_concatz(result, rest, NULL);
	bits_free(&rest);
	avma = ltop;
	return result;
}

bool brainpool_seed_valid(const char *hex_str) {
	const char *seed = str_is_hex(hex_str);
	return seed && strlen(seed) == 40;
}

GENERATOR(brainpool_gen_seed_random) {
	seed_t *seed = brainpool_new();
	seed->seed = bits_new_rand(160);
	seed_wv(seed);
	curve->seed = seed;
	return 1;
}

GENERATOR(brainpool_gen_seed_argument) {
	seed_t *seed = brainpool_new();
	seed->seed = bits_from_hex(str_is_hex(cfg->seed));
	seed_wv(seed);
	curve->seed = seed;
	return 1;
}

GENERATOR(brainpool_gen_seed_input) {
	pari_sp ltop = avma;

	GEN str = input_string("seed:");
	const char *cstr = GSTR(str);
	if (!brainpool_seed_valid(cstr)) {
		fprintf(err, "SEED must be exactly 160 bits(40 hex characters).\n");
		avma = ltop;
		return 0;
	}

	seed_t *seed = brainpool_new();
	seed->seed = bits_from_hex(str_is_hex(cstr));
	seed_wv(seed);
	curve->seed = seed;
	return 1;
}

GENERATOR(brainpool_gen_field) {
	pari_sp btop = avma;
	seed_t *seed = curve->seed;
	do {
		if (seed->brainpool.update_seed) {
			brainpool_update_seed(seed->seed);
			seed->brainpool.update_seed = false;
		}
		bits_t *p_bits = brainpool_hash(seed->seed, seed->brainpool.w + 1,
		                                seed->brainpool.v);
		GEN c = bits_to_i(p_bits);
		bits_free(&p_bits);
		GEN p = c;
		pari_sp bbtop = avma;
		do {
			if (p != c) {  // yes, check ptr identity here
				avma = bbtop;
			}
			p = nextprime(addii(p, gen_1));
		} while (mod4(p) != 3);

		GEN lower_bound = subii(int2u(cfg->bits - 1), gen_1);
		GEN upper_bound = int2u(cfg->bits);
		if (mpcmp(p, lower_bound) <= 0 || mpcmp(p, upper_bound) >= 0) {
			brainpool_update_seed(seed->seed);
			avma = btop;
			continue;
		}

		if (!isprime(p)) {
			brainpool_update_seed(seed->seed);
			avma = btop;
			continue;
		}

		curve->field = gcopy(p);
		gerepileall(btop, 1, &curve->field);
		break;
	} while (true);

	seed->brainpool.update_seed = true;
	return 1;
}

GENERATOR(brainpool_gen_equation) {
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

		brainpool_update_seed(seed->seed);

		bits_t *b_bits =
		    brainpool_hash(seed->seed, seed->brainpool.w, seed->brainpool.v);
		GEN b = bits_to_i(b_bits);
		bits_free(&b_bits);
		if (!Fp_issquare(b, curve->field)) {
			brainpool_update_seed(seed->seed);
			// bits_free(&seed->brainpool.seed_a);
			avma = btop;
			continue;
		}
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

GENERATOR(brainpool_gen_gens) {
	pari_sp ltop = avma;
	seed_t *seed = curve->seed;
	brainpool_update_seed(seed->seed);

	bits_t *k_bits =
	    brainpool_hash(seed->seed, seed->brainpool.w, seed->brainpool.v);
	GEN k = bits_to_i(k_bits);
	bits_free(&k_bits);
	GEN x = gen_0;
	GEN Qy = ellordinate(curve->curve, x, 0);
	while (glength(Qy) == 0) {
		x = addii(x, gen_1);
		Qy = ellordinate(curve->curve, x, 0);
	}

	GEN P = NULL;
	if (glength(Qy) == 1) {
		P = mkvec2(x, gel(Qy, 1));
	} else if (glength(Qy) == 2) {
		if (random_bits(1)) {
			P = mkvec2(x, gel(Qy, 1));
		} else {
			P = mkvec2(x, gel(Qy, 2));
		}
	} else {
		avma = ltop;
		return INT_MIN;
	}

	curve->generators = subgroups_new(1);
	curve->ngens = 1;
	subgroup_t *sub = subgroup_new();
	curve->generators[0] = sub;
	point_t *G = point_new();
	sub->generator = G;
	G->point = ellmul(curve->curve, P, k);
	G->order = ellorder(curve->curve, G->point, NULL);
	G->cofactor = divii(curve->order, G->order);
	seed->brainpool.mult = k;
	gerepileall(ltop, 4, &G->point, &G->order, &G->cofactor,
	            &seed->brainpool.mult);
	return 1;
}

CHECK(brainpool_check_gens) {
	pari_sp ltop = avma;
	point_t *G = curve->generators[0]->generator;
	GEN min_degree = divis(subii(G->order, gen_1), 100);
	if (mpcmp(min_degree, gens_get_embedding(curve->field, G->order)) >= 0) {
		avma = ltop;
		return -5;
	}
	avma = ltop;
	return 1;
}

CHECK(brainpool_check_order) {
	if (mpcmp(curve->order, curve->field) < 0) {
		return 1;
	} else {
		return -4;
	}
}