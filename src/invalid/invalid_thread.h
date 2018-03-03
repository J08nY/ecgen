/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file invalid_thread.h
 */
#ifndef ECGEN_INVALID_INVALID_THREAD_H
#define ECGEN_INVALID_INVALID_THREAD_H

#include <pthread.h>
#include "exhaustive/exhaustive.h"
#include "misc/types.h"

typedef enum { STATE_FREE, STATE_GENERATING, STATE_GENERATED } state_e;

typedef struct {
	struct pari_thread *pari_thread;
	const curve_t *original_curve;
	size_t nprimes;
	pari_ulong *primes;
	state_e *states;
	curve_t **curves;
	size_t *generated;
	pthread_mutex_t *mutex_state;
	pthread_cond_t *cond_generated;
	const config_t *cfg;
	const exhaustive_t *setup;
} thread_t;

/**
 *
 * @param arg
 * @return
 */
void *invalid_thread(void *arg);

#endif  // ECGEN_INVALID_INVALID_THREAD_H
