
#ifndef ECGEN_OBJ_POINT_H
#define ECGEN_OBJ_POINT_H

#include "misc/types.h"

/**
 *
 * @return
 */
point_t *point_new(void);

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
 * @return
 */
point_t *point_new_copy(const point_t *src);

/**
 *
 * @param src
 * @param dest
 * @return
 */
point_t *point_clone(const point_t *src, point_t *dest);

/**
 *
 * @param src
 * @return
 */
point_t *point_new_clone(const point_t *src);

/**
 *
 * @param point
 */
void point_free(point_t **point);

/**
 *
 * @param num
 * @return
 */
point_t **points_new(unsigned long num);

/**
 *
 * @param src
 * @param dest
 * @param num
 * @return
 */
point_t **points_copy(point_t **src, point_t **dest, unsigned long num);

/**
 *
 * @param src
 * @param num
 * @return
 */
point_t **points_new_copy(point_t **src, unsigned long num);

/**
 *
 * @param src
 * @param dest
 * @param num
 * @return
 */
point_t **points_clone(point_t **src, point_t **dest, unsigned long num);

/**
 *
 * @param src
 * @param num
 * @return
 */
point_t **points_new_clone(point_t **src, unsigned long num);

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
