/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
#include "timeout.h"
#include "util/memory.h"

__thread sigjmp_buf timeout_ptr;
__thread bool timeout_in = false;
__thread timer_t *timeout_timer;
__thread struct sigevent *sevp;

void timeout_handle(int signum, siginfo_t *siginfo, void *other) {
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGALRM);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);

	if (timeout_in) siglongjmp(timeout_ptr, 1);
}

void timeout_thread_init() {
	timeout_timer = try_calloc(sizeof(timer_t));
	sevp = try_calloc(sizeof(struct sigevent));
}

void timeout_thread_quit() {
	try_free(timeout_timer);
	try_free(sevp);
}

bool timeout_init() {
	// init for the main thread.
	timeout_thread_init();
	struct sigaction new_action;

	new_action.sa_sigaction = timeout_handle;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags = SA_SIGINFO;

	sigaction(SIGALRM, &new_action, NULL);
	return true;
}

void timeout_quit() {
	// deinit the main thread.
	timeout_thread_quit();
}
