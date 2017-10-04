/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_TIMEOUT_H
#define ECGEN_TIMEOUT_H

#include <setjmp.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>
#include "misc/config.h"

extern __thread jmp_buf exception;

#define timeout_start(seconds)                                  \
	do {                                                        \
		struct sigevent sevp;                                   \
		sevp.sigev_notify = SIGEV_THREAD_ID;                    \
		sevp.sigev_signo = SIGALRM;                             \
		sevp._sigev_un._tid = (__pid_t)syscall(SYS_gettid);     \
                                                                \
		timer_t timer;                                          \
		timer_create(CLOCK_MONOTONIC, &sevp, &timer);           \
		struct itimerspec timer_time = {                        \
		    .it_interval = {.tv_sec = (seconds), .tv_nsec = 0}, \
		    .it_value = {.tv_sec = 0, .tv_nsec = 0}};           \
		timer_settime(timer, 0, &timer_time, NULL);             \
		setjmp(exception);                                      \
	} while (0);

/**
 * @brief
 * @param cfg
 * @return
 */
bool timeout_init(const config_t *cfg);

#endif  // ECGEN_TIMEOUT_H
