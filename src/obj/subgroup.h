
#ifndef ECGEN_OBJ_SUBGROUP_H
#define ECGEN_OBJ_SUBGROUP_H

#include "misc/types.h"
#include "obj.h"

OBJ_H(subgroup, subgroup_t)
OBJS_H(subgroup, subgroup_t)

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
 * @param dst
 * @return
 */
subgroup_t *subgroup_clone(const subgroup_t *src, subgroup_t *dst);

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
