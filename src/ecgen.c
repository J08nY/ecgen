/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */
/**
 * @author J08nY <johny@neuromancer.sk>
 * @license GPL v2.0
 * @version 0.2
 */

#include <time.h>
#include "cli.h"
#include "curve.h"
#include "output.h"
#include "input.h"

const char *argp_program_version =
    "ecgen 0.2\n"
    "Copyright (C) 2017 J08nY\n"
    "License GPLv2: GNU GPL version 2 (or later) "
    "<http://gnu.org/licenses/gpl.html>\n"
    "This is free software: you are free to change and redistribute it.\n"
    "There is NO WARRANTY, to the extent permitted by law.";
const char *argp_program_bug_address = "<johny@neuromancer.sk>";

static struct argp argp = {options, parse_opt, args_doc, doc};
static struct config_t cfg;
static FILE *in;
static FILE *out;

bool init() {
	// Init PARI, 1GB stack, 1M primes
	pari_init(1000000000, 1000000);

	// Init PARI PRNG
	pari_ulong seed = 0;
	// Try urandom first
	FILE *rand = fopen("/dev/urandom", "rb");
	if (rand) {
		fread(&seed, sizeof(char), sizeof(pari_ulong), rand);
		fclose(rand);
	}
	// Try worse methods later
	if (seed == 0) {
		struct timespec t;
		if (!clock_gettime(CLOCK_REALTIME, &t)) {
			seed = (pari_ulong)t.tv_nsec;
		} else {
			seed = (pari_ulong)time(NULL);
		}
	}

	pari_sp ltop = avma;
	setrand(utoi(seed));
	avma = ltop;

	// set datadir if specified
	if (cfg.datadir) {
		default0("datadir", cfg.datadir);
	}

	// open outfile
	out = output_open(cfg.output, cfg.append);

	// open infile
	in = input_open(cfg.input);

	return true;
}

int quit(int status) {
	pari_close();

	output_close(out);
	input_close(in);

	return status;
}

int main(int argc, char *argv[]) {
	// Parse cli args
	memset(&cfg, 0, sizeof(cfg));
	argp_parse(&argp, argc, argv, 0, 0, &cfg);

	if (!init()) {
		return quit(1);
	}

	if (cfg.random) {
		curve_t *curve;
		if (cfg.prime) {
			curve = curve_primef(cfg.field, cfg.bits);
		} else {
			curve = curve_nonzerof(cfg.field, cfg.bits);
		}

		output_csv(out, "%Px", ',', curve_params(curve));

		free_curve(&curve);

	} else {
		fprintf(stderr, "Currently unsupported.");
	}

	return quit(0);
}
