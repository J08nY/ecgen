/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "timeout.h"
#include <signal.h>

__thread jmp_buf exception;

void timeout_handle(int signum) { longjmp(exception, 1); }

bool timeout_init(const config_t *cfg) {
	struct sigaction new_action;

	new_action.sa_handler = timeout_handle;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags = 0;

	sigaction(SIGALRM, &new_action, NULL);
	return true;
}