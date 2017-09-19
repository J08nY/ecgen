
#include <io/config.h>
#include <gen/types.h>
#include "ansi.h"
#include "gen/seed.h"
#include "gen/field.h"
#include "util/bits.h"
#include "util/memory.h"
#include "io/output.h"


bool ansi_seed_valid(const char *hex_str) {
	size_t len = strlen(hex_str);
	if (len < 40) {
		return false;
	}
	const char *str_start = hex_str;
	if (hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X')) {
		str_start = hex_str + 2;
	}
	while (*str_start != 0) {
		char c = *str_start++;
		if (!isxdigit(c)) return false;
	}
	return true;
}

static bits_t *seed_stoi(const char *cstr) {
	const char *seed_str = cstr;
	const char *prefix = strstr(cstr, "0x");
	if (prefix != NULL) seed_str = prefix + 2;
	return bits_from_hex(seed_str);
}

static void seed_hash(seed_t *seed) {
	seed->hash20 = try_malloc(20);
	bits_sha1(seed->seed, seed->hash20);
}

static void seed_tsh(seed_t *seed, const config_t *cfg) {
	pari_sp ltop = avma;
	seed->ansi.t = utoi(cfg->bits);
	seed->ansi.s = floorr(rdivii(subis(seed->ansi.t, 1), stoi(160), DEFAULTPREC));
	seed->ansi.h = subii(seed->ansi.t, mulis(seed->ansi.s, 160));
	gerepileall(ltop, 3, &seed->ansi.t, &seed->ansi.s, &seed->ansi.h);
}

GENERATOR(ansi_gen_seed_random) {
	seed_t *seed = seed_new();
	seed->seed = bits_from_i(random_int(160));
	seed_hash(seed);
	seed_tsh(seed, cfg);
	curve->seed = seed;
	return 1;
}

GENERATOR(ansi_gen_seed_argument) {
	seed_t *seed = seed_new();
	seed->seed = seed_stoi(cfg->seed);
	seed_hash(seed);
	seed_tsh(seed, cfg);
	curve->seed = seed;
	return 1;
}

GENERATOR(ansi_gen_seed_input) {
	pari_sp ltop = avma;

	GEN str = input_string("seed:");
	const char *cstr = GSTR(str);
	if (!ansi_seed_valid(cstr)) {
		fprintf(err, "SEED must be at least 160 bits(40 hex characters).\n");
		avma = ltop;
		return 0;
	}

	seed_t *seed = seed_new();
	seed->seed = seed_stoi(cstr);
	seed_hash(seed);
	seed_tsh(seed, cfg);
	curve->seed = seed;
	return 1;
}

static bits_t *seed_process(seed_t *seed, const bits_t *first) {
	pari_sp ltop = avma;

	bits_t *result = bits_copy(first);

	long is = itos(seed->ansi.s);
	GEN seedi = bits_to_i(seed->seed);
	GEN two_g = int2n(seed->seed->bitlen);

	for (long i = 1; i <= is; ++i) {
		pari_sp btop = avma;
		GEN inner = addis(seedi, i);
		inner = modii(inner, two_g);

		bits_t *to_hash = bits_from_i(inner);
		if (to_hash->bitlen < seed->seed->bitlen) {
			bits_lengthenz(to_hash, seed->seed->bitlen - to_hash->bitlen);
		}
		unsigned char hashout[20];
		bits_sha1(to_hash, hashout);
		bits_t *Wi = bits_from_raw(hashout, 160);
		bits_concatz(result, Wi, NULL);

		bits_free(&to_hash);
		bits_free(&Wi);
		avma = btop;
	}

	avma = ltop;
	return result;
}

UNROLL(ansi_unroll_seed) {
	seed_free(&curve->seed);
	return -1;
}

static GENERATOR(ansi_gen_equation_fp) {
	bits_t *c0 = bits_from_raw(curve->seed->hash20, 160);
	bits_shortenz(c0, 160 - itos(curve->seed->ansi.h));

	bits_t *W0 = bits_copy(c0);
	SET_BIT(W0->bits, 0, 0);

	bits_t *W = seed_process(curve->seed, W0);

	return 0;
}

static GENERATOR(ansi_gen_equation_f2m) {
	bits_t *b0 = bits_from_raw(curve->seed->hash20, 160);
	bits_shortenz(b0, 160 - itos(curve->seed->ansi.h));

	bits_t *b = seed_process(curve->seed, b0);
	GEN ib = bits_to_i(b);
	if (gequal0(ib)) {
		return -3;
	}
	GEN a = random_int(cfg->bits);
	curve->a = field_ielement(curve->field, a);
	curve->b = field_ielement(curve->field, ib);
	return 1;
}

GENERATOR(ansi_gen_equation) {
	switch (cfg->field) {
		case FIELD_PRIME:
			return ansi_gen_equation_fp(curve, cfg, args);
		case FIELD_BINARY:
			return ansi_gen_equation_f2m(curve, cfg, args);
		default:
			pari_err_BUG("Field not prime or binary?");
			return INT_MIN; /* NOT REACHABLE */
	}
}
