
#ifndef ECGEN_OBJ_POINT_H
#define ECGEN_OBJ_POINT_H

#include "misc/types.h"
#include "obj.h"

OBJ_H(point, point_t)
OBJS_H(point, point_t)

/**
 *
 * @param src
 * @param dest
 * @return
 */
point_t *point_copy(const point_t *src, point_t *dest);

/**
 *
 * @param src
 * @param dest
 * @return
 */
point_t *point_clone(const point_t *src, point_t *dest);

/**
 *
 * @param point
 */
void point_free(point_t **point);

/**
 *
 * @param point
 */
void points_free(point_t ***point);

/**
 *
 * @param points
 * @param npoints
 */
void points_free_deep(point_t ***points, unsigned long npoints);

#endif  // ECGEN_OBJ_POINT_H
