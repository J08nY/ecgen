
#include <io/config.h>
#include "ansi.h"
#include "gen/seed.h"
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

static void seed_W(seed_t *seed, const config_t *cfg) {
	pari_sp ltop = avma;
	GEN t = utoi(cfg->bits);
	GEN s = floorr(rdivii(subis(t, 1), stoi(160), DEFAULTPREC));
	GEN h = subii(t, mulis(s, 160));
	pari_printf("bits = %lu, t = %Pi, s = %Pi, h = %Pi\n", cfg->bits, t, s, h);

	bits_t *c0 = bits_from_raw(seed->hash20, 160);
	printf("H = %s, len = %lu alloc = %lu\n", bits_to_hex(c0), c0->bitlen,
		   c0->allocated);
	bits_shortenz(c0, 160 - itos(h));
	printf("c0 = %s\n", bits_to_hex(c0));

	bits_t *W0 = bits_copy(c0);
	SET_BIT(W0->bits, 0, 0);

	long is = itos(s);
	seed->W = bits_copy(W0);
	GEN two_g = int2n(seed->seed->bitlen);
	for (long i = 1; i <= is; ++i) {
		printf("doing i = %li\n", i);
		pari_sp btop = avma;
		GEN inner = bits_to_i(seed->seed);
		inner = addis(inner, i);
		inner = modii(inner, two_g);

		bits_t *to_hash = bits_from_i(inner);
		unsigned char hashout[20];
		bits_sha1(to_hash, hashout);
		bits_t *Wi = bits_from_raw(hashout, 160);
		bits_concatz(seed->W, Wi, NULL);
		bits_free(&to_hash);
		bits_free(&Wi);
		avma = btop;
	}

	bits_free(&c0);
	bits_free(&W0);
	avma = ltop;
}

GENERATOR(ansi_gen_seed_random) {
	seed_t *seed = seed_new();
	seed->seed = bits_from_i(random_int(160));
	seed_hash(seed);
	seed_W(seed, cfg);
	curve->seed = seed;
	return 1;
}

GENERATOR(ansi_gen_seed_argument) {
	seed_t *seed = seed_new();
	seed->seed = seed_stoi(cfg->seed);
	seed_hash(seed);
	seed_W(seed, cfg);
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
	seed_W(seed, cfg);
	curve->seed = seed;
	return 1;
}

static GENERATOR(ansi_gen_equation_fp) {
	return 0;
}

static GENERATOR(ansi_gen_equation_f2m) {
	return 0;
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
