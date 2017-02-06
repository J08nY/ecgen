/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "input.h"

/*
 *
 *char *prime_prompts[] = {"p:", "a:", "b:"};
    param_t prime_params[] = {PARAM_PRIME, PARAM_INT, PARAM_INT};

    char *binary_prompts[] = {"e1:", "e2:", "e3:", "a:", "b:"};
    param_t binary_params[] = {PARAM_SHORT, PARAM_SHORT, PARAM_SHORT, PARAM_INT,
 PARAM_INT};

    char **prompts;
    param_t *params;
    size_t length;
    if (cfg.prime_field) {
        prompts = prime_prompts;
        params = prime_params;
        length = 3;
    } else {
        prompts = binary_prompts;
        params = binary_params;
        length = 5;
    }
    GEN field;
    GEN domain[length];

    if (cfg.random) {
        //random domain, might not define a curve... check disc
        if (cfg.prime_field) {
            field = ifield_prime(cfg.bits);
        } else {
            field = ifield_binary(cfg.bits);
        }
    } else {
        for (size_t i = 0; i < length; ++i) {
            domain[i] = fread_param(params[i], in, prompts[i], cfg.bits, in ==
 stdin ? '\n' : ',');
            if (equalii(domain[i], gen_m1)) {
                fprintf(stderr, "Whoops?");
                return quit(1);
            }
        }
        if (cfg.prime_field) {
            field = field_prime(domain[0]);
        } else {
            field = field_binary(cfg.bits, domain[0], domain[1], domain[2]);
        }
    }
    pari_fprintf(out, "%Ps", field_params(field));

    if (cfg.prime_field) {
        GEN field = prime_field(p);
        GEN curve = prime_weierstrass(a, b, field, 0);
    } else if (cfg.binary_field) {
        GEN e[3];
        for (size_t i = 0; i < 3; ++i) {
            char prompt[] = {'e', (char) ('1' + i), ':', 0};
            e[i] = read_short(prompt, '\n');
        }
        GEN a = read_int("a:", cfg.bits, '\n');

        GEN field = binary_field(cfg.bits, e[0], e[1], e[2]);
    }
*/

GEN fread_i(FILE *stream, const char *prompt, long bits, int delim,
            GEN (*rand_func)(long)) {
	printf("%s ", prompt);
	char *line = NULL;
	size_t n = 0;

	ssize_t len = getdelim(&line, &n, delim, stream);
	if (len == 1) {
		free(line);
		if (rand_func) {
			return rand_func(bits);
		} else {
			return gen_0;
		}
	}
	pari_sp ltop = avma;
	GEN in = strtoi(line);
	free(line);

	// check bitsize here
	GEN size = int2n(bits);
	if (cmpii(in, size)) {
		return gerepileupto(ltop, in);
	} else {
		fprintf(stderr, "Number too big(> %ld bits).\n", bits);
		return gen_m1;
	}
}

GEN fread_prime(FILE *stream, const char *prompt, long bits, int delim) {
	GEN read = fread_i(stream, prompt, bits, delim, &random_prime);
	if (equalii(read, gen_m1)) {
		return read;
	} else {
		if (isprime(read)) {
			return read;
		} else {
			fprintf(stderr, "Number is not prime. Prime required.\n");
			return gen_m1;
		}
	}
}

GEN fread_int(FILE *stream, const char *prompt, long bits, int delim) {
	return fread_i(stream, prompt, bits, delim, &random_int);
}

GEN fread_short(FILE *stream, const char *prompt, int delim) {
	return fread_i(stream, prompt, 16, delim, NULL);
}

GEN fread_param(param_t param, FILE *stream, const char *prompt, long bits,
                int delim) {
	switch (param) {
		case PARAM_PRIME:
			return fread_prime(stream, prompt, bits, delim);
		case PARAM_INT:
			return fread_int(stream, prompt, bits, delim);
		case PARAM_SHORT:
			return fread_short(stream, prompt, delim);
	}
	return gen_m1;
}

GEN read_param(param_t param, const char *prompt, long bits, int delim) {
	return fread_param(param, stdin, prompt, bits, delim);
}

FILE *input_open(const char *input) {
	if (input) {
		FILE *in = fopen(input, "r");
		if (!in) {
			// fallback to stdin or quit?
			in = stdin;
			perror("Failed to open input file.");
		}
		return in;
	} else {
		return stdin;
	}
}

void input_close(FILE *in) {
	if (in != NULL && in != stdout) {
		fclose(in);
	}
}
