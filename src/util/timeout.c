/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "timeout.h"

__thread jmp_buf timeout_ptr;
__thread bool timeout_in = false;
__thread timer_t timeout_timer;

void timeout_handle(int signum, siginfo_t *siginfo, void *other) {
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGALRM);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);

	if (timeout_in) siglongjmp(timeout_ptr, 1);
}

bool timeout_init(const config_t *cfg) {
	struct sigaction new_action;

	new_action.sa_sigaction = timeout_handle;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags = SA_SIGINFO;

	sigaction(SIGALRM, &new_action, NULL);
	return true;
}