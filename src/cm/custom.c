/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "custom.h"
#include "io/input.h"

static bool custom_is_disc(GEN test) {
	bool result = true;
	long md4;
	pari_CATCH(e_DOMAIN)
			{
				result = false;
			}
		pari_TRY{ check_quaddisc_imag(test, &md4, ""); };
	pari_ENDCATCH
	return result;
}

static GEN custom_disc(GEN order, GEN prime) {
	pari_sp ltop = avma;

	GEN t = subii(order, addii(prime, gen_1));
	GEN v2D = subii(mulis(prime, 4), sqri(t));

	GEN max_divisor = gen_1;
	GEN min_D = v2D;

	// Optional part that sometimes makes the discriminant smaller, at the cost
	// factoring v2D.
	GEN fctr = factor(v2D);
	long len = glength(gel(fctr, 1));
	for (long i = 1; i <= len; ++i) {
		GEN value = gmael(fctr, 1, i);
		GEN mul = gmael(fctr, 2, i);
		GEN pow;
		if (mod2(mul) == 0) {
			pow = mul;
		} else {
			pow = subis(mul, 1);
		}
		if (equalis(pow, 0)) {
			continue;
		}
		GEN divi = powii(value, pow);
		if (mpcmp(divi, max_divisor) > 0) {
			GEN candidate_D = divii(v2D, divi);
			if (signe(candidate_D) == 1) {
				setsigne(candidate_D, -1);
			}
			if (custom_is_disc(candidate_D)) {
				max_divisor = divi;
				min_D = candidate_D;
			}
		}
	}

	if (signe(min_D) == 1) {
		setsigne(min_D, -1);
	}
	if (custom_is_disc(min_D)) {
		return gerepileupto(ltop, min_D);
	} else {
		avma = ltop;
		return NULL;
	}
}

static custom_quadr_t custom_prime_input(GEN order) {
	pari_sp ltop = avma;
	custom_quadr_t result = {0};
	GEN p;
	GEN D;
	do {
		p = input_prime("p:", cfg->bits);
		if (gequalm1(p)) {
			continue;
		}
		D = custom_disc(order, p);
	} while (D == NULL);

	gerepileall(ltop, 2, &p, &D);
	result.p = p;
	result.D = D;
	return result;
}

static custom_quadr_t custom_prime_random(GEN order) {
	pari_sp ltop = avma;
	custom_quadr_t result = {0};
	GEN p;
	GEN t = gen_0;
	GEN v = gen_0;
	GEN D;
	long i = 2;
	//TODO: this is wrong, order = p + 1 +- t  is not kept.
	while (gequal0(t)) {
		D = stoi(-4 * (i / 2) + (i % 2));
		for (long j = 0; j < 100; ++j) {
			p = random_prime(cfg->bits);
			if (cornacchia2(negi(D), p, &t, &v)) {
				break;
			}
		}
	};

	gerepileall(ltop, 4, &p, &t, &v, &D);
	result.p = p;
	result.t = t;
	result.v = v;
	result.D = D;
	return result;
}

curve_t *custom_curve() {
	GEN order = strtoi(cfg->cm_order);

	custom_quadr_t quadr;
	if (cfg->random) {
		quadr = custom_prime_random(order);
	} else {
		quadr = custom_prime_input(order);
	}
	pari_printf("p = %Pi, D = %Pi, ", quadr.p, quadr.D);
	GEN H = polclass(quadr.D, 0, 0);
	pari_printf("H = %Ps\n", H);

	return NULL;
}