/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file timeout.h
 */
#ifndef ECGEN_TIMEOUT_H
#define ECGEN_TIMEOUT_H

#include <setjmp.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>
#include "io/output.h"
#include "misc/config.h"

extern __thread sigjmp_buf timeout_ptr;
extern __thread bool timeout_in;
extern __thread timer_t timeout_timer;

/**
 * @brief
 */
#define timeout_start(seconds)                                \
	if ((seconds) != 0) {                                     \
		struct sigevent sevp;                                 \
		sevp.sigev_notify = SIGEV_THREAD_ID;                  \
		sevp.sigev_signo = SIGALRM;                           \
		sevp._sigev_un._tid = (__pid_t)syscall(SYS_gettid);   \
                                                              \
		timer_create(CLOCK_MONOTONIC, &sevp, &timeout_timer); \
		struct itimerspec timer_time = {                      \
		    .it_interval = {.tv_sec = 0, .tv_nsec = 0},       \
		    .it_value = {.tv_sec = (seconds), .tv_nsec = 0}}; \
		timer_settime(timeout_timer, 0, &timer_time, NULL);   \
		timeout_in = true;                                    \
	};                                                        \
	if ((seconds) != 0 && sigsetjmp(timeout_ptr, 1) == 1)

/**
 * @brief
 */
#define timeout_stop()                   \
	{                                    \
		if (timeout_in) {                \
			timeout_in = false;          \
			timer_delete(timeout_timer); \
		}                                \
	}

/**
 * @brief
 * @param cfg
 * @return
 */
bool timeout_init(const config_t *cfg);

#endif  // ECGEN_TIMEOUT_H
