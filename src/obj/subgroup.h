
#ifndef ECGEN_OBJ_SUBGROUP_H
#define ECGEN_OBJ_SUBGROUP_H

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

#endif  // ECGEN_OBJ_SUBGROUP_H
