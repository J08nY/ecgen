/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <criterion/criterion.h>
#include <unistd.h>
#include "io/cli.h"
#include "misc/config.h"
#include "test/default.h"

static struct argp test_argp = {cli_options, cli_parse, cli_args_doc,
                                cli_doc,     0,         cli_filter};

TestSuite(cli, .init = default_setup, .fini = default_teardown);

Test(cli, test_memory) {
	int argc = 4;
	char *argv[] = {"ecgen", "--memory=2k", "--fp", "1"};
	int ret = argp_parse(&test_argp, argc, argv, 0, 0, cfg);
	cr_assert_eq(ret, 0, );
	cr_assert_eq(cfg->memory, 2000, );

	char *argx[] = {"ecgen", "--memory=2m", "--fp", "1"};
	ret = argp_parse(&test_argp, argc, argx, 0, 0, cfg);
	cr_assert_eq(ret, 0, );
	cr_assert_eq(cfg->memory, 2000000, );

	char *argy[] = {"ecgen", "--memory=2g", "--fp", "1"};
	ret = argp_parse(&test_argp, argc, argy, 0, 0, cfg);
	cr_assert_eq(ret, 0, );
	cr_assert_eq(cfg->memory, 2000000000, );
}

Test(cli, test_thread_memory) {
	int argc = 4;
	char *argv[] = {"ecgen", "--thread-stack=2k", "--fp", "1"};
	int ret = argp_parse(&test_argp, argc, argv, 0, 0, cfg);
	cr_assert_eq(ret, 0, );
	cr_assert_eq(cfg->thread_memory, 2000, );
}

Test(cli, test_threads) {
	int argc = 4;
	char *argv[] = {"ecgen", "--threads=2", "--fp", "1"};
	int ret = argp_parse(&test_argp, argc, argv, 0, 0, cfg);
	cr_assert_eq(ret, 0, );
	cr_assert_eq(cfg->threads, 2, );
}

Test(cli, test_auto_threads) {
	int argc = 4;
	char *argv[] = {"ecgen", "--threads=auto", "--fp", "1"};
	int ret = argp_parse(&test_argp, argc, argv, 0, 0, cfg);
	cr_assert_eq(ret, 0, );
	cr_assert_eq(cfg->threads, sysconf(_SC_NPROCESSORS_ONLN), );
}

Test(cli, test_timeout) {
	int argc = 4;
	char *argv[] = {"ecgen", "--timeout=10m", "--fp", "1"};
	int ret = argp_parse(&test_argp, argc, argv, 0, 0, cfg);
	cr_assert_eq(ret, 0, );
	cr_assert_eq(cfg->timeout, 600, );

	char *argx[] = {"ecgen", "--timeout=1h", "--fp", "1"};
	ret = argp_parse(&test_argp, argc, argx, 0, 0, cfg);
	cr_assert_eq(ret, 0, );
	cr_assert_eq(cfg->timeout, 3600, );

	char *argy[] = {"ecgen", "--timeout=1d", "--fp", "1"};
	ret = argp_parse(&test_argp, argc, argy, 0, 0, cfg);
	cr_assert_eq(ret, 0, );
	cr_assert_eq(cfg->timeout, 86400, );
}