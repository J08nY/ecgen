/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017-2018 J08nY
 */
/**
 * @file subgroup.h
 */
#ifndef ECGEN_SUBGROUPS_H
#define ECGEN_SUBGROUPS_H

#include <pari/pari.h>
#include "misc/types.h"

/**
 * @brief
 * @return
 */
subgroup_t *subgroup_new(void);

/**
 * @brief
 * @param src
 * @param dst
 * @return
 */
subgroup_t *subgroup_copy(const subgroup_t *src, subgroup_t *dst);

/**
 * @brief
 * @param src
 * @return
 */
subgroup_t *subgroup_new_copy(const subgroup_t *src);

/**
 * @brief
 * @param src
 * @param dst
 * @return
 */
subgroup_t *subgroup_clone(const subgroup_t *src, subgroup_t *dst);

/**
 * @brief
 * @param src
 * @return
 */
subgroup_t *subgroup_new_clone(const subgroup_t *src);

/**
 * @brief
 * @param subgroup
 */
void subgroup_free(subgroup_t **subgroup);

/**
 * @brief
 * @param subgroup
 */
void subgroup_free_deep(subgroup_t **subgroup);

/**
 * @brief
 * @param num
 * @return
 */
subgroup_t **subgroups_new(size_t num);

/**
 *
 * @param src
 * @param dest
 * @param num
 * @return
 */
subgroup_t **subgroups_copy(subgroup_t **src, subgroup_t **dest, size_t num);

/**
 *
 * @param src
 * @param num
 * @return
 */
subgroup_t **subgroups_new_copy(subgroup_t **src, size_t num);

/**
 *
 * @param src
 * @param dest
 * @param num
 * @return
 */
subgroup_t **subgroups_clone(subgroup_t **src, subgroup_t **dest, size_t num);

/**
 *
 * @param src
 * @param num
 * @return
 */
subgroup_t **subgroups_new_clone(subgroup_t **src, size_t num);

/**
 * @brief
 * @param subgroups
 */
void subgroups_free(subgroup_t ***subgroups);

/**
 * @brief
 * @param subgroups
 * @param num
 */
void subgroups_free_deep(subgroup_t ***subgroups, size_t num);

/**
 * @brief All prime factors of a given integer, without multipliticity.
 *
 * subgroups_factors(27) = [3]
 * @param order
 * @return a t_VEC of prime factors.
 */
GEN subgroups_prime(GEN order);

/**
 * @brief All nonprime subgroup orders of a given integer.
 * @param order
 * @return a t_VEC of nonprime factors.
 */
GEN subgroups_nonprime(GEN order);

/**
 * @brief All all subgroup orders of a given integer.
 * @param order
 * @return a t_VEC of all factors.
 */
GEN subgroups_all(GEN order);

#endif  // ECGEN_SUBGROUPS_H
