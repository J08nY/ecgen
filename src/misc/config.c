/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "config.h"
#include <stdio.h>
#include <stdlib.h>

config_t cfg_s;
config_t *cfg = &cfg_s;

config_names_t cfg_used_s = {0};
config_names_t *cfg_used = &cfg_used_s;

config_names_t cfg_set_s = {0};
config_names_t *cfg_set = &cfg_set_s;

void config_report_unused() {
	bool unused = false;
	if (cfg_set->field && !cfg_used->field) {
		fprintf(
		    stderr,
		    "Warning: Ignored command-line argument \"field\" (--fp/--f2m).\n");
		unused = true;
	}
	if (cfg_set->method && !cfg_used->method) {
		fprintf(stderr, "Warning: Ignored command-line argument method.\n");
		unused = true;
	}
	if (cfg_set->count && !cfg_used->count) {
		fprintf(stderr,
		        "Warning: Ignored command-line argument count (-c/--count).\n");
		unused = true;
	}
	if (cfg_set->random && !cfg_used->random) {
		fprintf(
		    stderr,
		    "Warning: Ignored command-line argument random (-r/--random).\n");
		unused = true;
	}
	if (cfg_set->prime && !cfg_used->prime) {
		fprintf(stderr,
		        "Warning: Ignored command-line argument prime (-p/--prime).\n");
		unused = true;
	}
	if (cfg_set->cm_order && !cfg_used->cm_order) {
		fprintf(
		    stderr,
		    "Warning: Ignored command-line argument cm_order (-n/--order).\n");
		unused = true;
	}
	if (cfg_set->koblitz && !cfg_used->koblitz) {
		fprintf(
		    stderr,
		    "Warning: Ignored command-line argument koblitz (-K/--koblitz).\n");
		unused = true;
	}
	if (cfg_set->koblitz_value && !cfg_used->koblitz_value) {
		fprintf(stderr,
		        "Warning: Ignored command-line argument koblitz_value "
		        "(-K/--koblitz).\n");
		unused = true;
	}
	if (cfg_set->smooth && !cfg_used->smooth) {
		fprintf(
		    stderr,
		    "Warning: Ignored command-line argument smooth (-B/--smooth).\n");
		unused = true;
	}
	if (cfg_set->cofactor && !cfg_used->cofactor) {
		fprintf(stderr,
		        "Warning: Ignored command-line argument cofactor "
		        "(-k/--cofactor).\n");
		unused = true;
	}
	if (cfg_set->invalid_primes && !cfg_used->invalid_primes) {
		fprintf(stderr,
		        "Warning: Ignored command-line argument invalid_primes "
		        "(-i/--invalid).\n");
		unused = true;
	}
	if (cfg_set->seed_algo && !cfg_used->seed_algo) {
		fprintf(
		    stderr,
		    "Warning: Ignored command-line argument seed_algo (-s/--ansi).\n");
		unused = true;
	}
	if (cfg_set->seed && !cfg_used->seed) {
		fprintf(stderr,
		        "Warning: Ignored command-line argument seed (-s/--ansi).\n");
		unused = true;
	}
	if (cfg_set->unique && !cfg_used->unique) {
		fprintf(
		    stderr,
		    "Warning: Ignored command-line argument unique (-u/--unique).\n");
		unused = true;
	}
	if (cfg_set->hex_check && !cfg_used->hex_check) {
		fprintf(stderr,
		        "Warning: Ignored command-line argument hex_check "
		        "(--hex-check).\n");
		unused = true;
	}
	if (cfg_set->points && !cfg_used->points) {
		fprintf(stderr,
		        "Warning: Ignored command-line argument points (--points).\n");
		unused = true;
	}
	if (cfg_set->metadata && !cfg_used->metadata) {
		fprintf(
		    stderr,
		    "Warning: Ignored command-line argument metadata (--metadata).\n");
		unused = true;
	}
#if DEBUG
	if (unused) {
		exit(42);
	}
#else
	(void)unused;
#endif
}