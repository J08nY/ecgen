
#ifndef ECGEN_OBJ_CURVE_H
#define ECGEN_OBJ_CURVE_H

#include "misc/types.h"

/**
 * Allocates and zeros out a new curve_t object.
 * @return new curve
 */
curve_t *curve_new(void);

/**
 * Copies parameters from src curve to dest curve, allocates space for points.
 * Otherwise expects everything to be allocated.
 *
 * @param src source curve
 * @param dest destination curve
 * @return destination curve
 */
curve_t *curve_copy(const curve_t *src, curve_t *dest);

/**
 *
 * @param src
 * @return
 */
curve_t *curve_new_copy(const curve_t *src);

/**
 *
 * @param src
 * @param dest
 * @return
 */
curve_t *curve_clone(const curve_t *src, curve_t *dest);

/**
 *
 * @param src
 * @return
 */
curve_t *curve_new_clone(const curve_t *src);

/**
 * Free a curve_t along with it's seed_t and point_ts.
 * @param curve to free
 */
void curve_free(curve_t **curve);

#include "misc/types.h"

#endif  // ECGEN_OBJ_CURVE_H
