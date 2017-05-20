/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file cli.h
 */
#ifndef ECGEN_CLI_H
#define ECGEN_CLI_H

#include <argp.h>
#include <stdbool.h>
#include <stdlib.h>

extern char cli_doc[];
extern char cli_args_doc[];
extern struct argp_option cli_options[];

error_t cli_parse(int key, char *arg, struct argp_state *state);

char *cli_filter(int key, const char *text, void *input);

#endif  // ECGEN_CLI_H
