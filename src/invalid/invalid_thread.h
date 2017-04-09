/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file invalid_thread.h
 */
#ifndef ECGEN_INVALID_THREAD_H
#define ECGEN_INVALID_THREAD_H

#include <pari/pari.h>
#include <pthread.h>
#include "math/types.h"

typedef enum { STATE_FREE, STATE_GENERATING, STATE_GENERATED } state_e;

typedef struct {
	curve_t *original_curve;
	size_t nprimes;
	pari_ulong *primes;
	state_e *states;
	curve_t **curves;
	size_t *generated;
	pthread_mutex_t *mutex_state;
	pthread_cond_t *cond_generated;
	config_t *cfg;
	gen_t *gens;
} thread_t;

/**
 *
 * @param arg
 * @return
 */
void *invalid_thread(void *arg);

#endif  // ECGEN_INVALID_THREAD_H