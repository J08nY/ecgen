/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file timeout.h
 */
#ifndef ECGEN_UTIL_TIMEOUT_H
#define ECGEN_UTIL_TIMEOUT_H

#include <setjmp.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>
#include "io/output.h"
#include "misc/config.h"

#ifdef __APPLE__

#include <posix-macos-timer.h>
#define SIGEV_THREAD_ID 0
#define SIGEV_UN_TID_SYS_GETTID

#else

#define SIGEV_UN_TID_SYS_GETTID \
	sevp->_sigev_un._tid = (__pid_t)syscall(SYS_gettid);

#endif

extern __thread sigjmp_buf timeout_ptr;
extern __thread bool timeout_in;
extern __thread timer_t *timeout_timer;
extern __thread struct sigevent *sevp;

/**
 * @brief Start a timer that times out after <code>seconds-</code>.
 *
 * The timer is thread-local. This function is an if-like statement,
 * it runs the <code>else</code> part for the duration of the timeout or
 * until <code>timeout_stop</code> is called. If that branch times out, then the
 * <code>if</code> part is run. The times should be always stopped by
 * <code>timeout_stop</code> when the timed interval ends.
 * @param seconds how long to run the <code>else</code> branch for
 */
#define timeout_start(seconds)                                \
	if ((seconds) != 0) {                                     \
		sevp->sigev_notify = SIGEV_THREAD_ID;                 \
		sevp->sigev_signo = SIGALRM;                          \
		sevp->sigev_value.sival_int = 0;                      \
		SIGEV_UN_TID_SYS_GETTID                               \
		timer_create(CLOCK_MONOTONIC, sevp, timeout_timer);   \
		struct itimerspec timer_time = {                      \
		    .it_interval = {.tv_sec = 0, .tv_nsec = 0},       \
		    .it_value = {.tv_sec = (seconds), .tv_nsec = 0}}; \
		timer_settime(*timeout_timer, 0, &timer_time, NULL);  \
		timeout_in = true;                                    \
	};                                                        \
	if ((seconds) != 0 && sigsetjmp(timeout_ptr, 1) == 1)

/**
 * @brief Stop a timer.
 */
#define timeout_stop()                    \
	{                                     \
		if (timeout_in) {                 \
			timeout_in = false;           \
			timer_delete(*timeout_timer); \
		}                                 \
	}

void timeout_thread_init();

void timeout_thread_quit();

/**
 * @brief Initialize the timeout system.
 * @return whether the initalization was successful
 */
bool timeout_init();

/**
 * @brief Deinitialize the timeout system.
 */
void timeout_quit();

#endif  // ECGEN_UTIL_TIMEOUT_H
