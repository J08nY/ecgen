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
#include <stdlib.h>

extern char doc[];
extern char args_doc[];
extern struct argp_option options[];

error_t cli_parse(int key, char *arg, struct argp_state *state);

char *cli_filter(int key, const char *text, void *input);

#endif  // ECGEN_CLI_H
