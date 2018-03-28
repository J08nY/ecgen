
#include "point.h"

OBJ(point, point_t, point_copy, point_clone)
OBJS(point, point_t, point_copy, point_clone)

point_t *point_copy(const point_t *src, point_t *dest) {
	if (src->point) dest->point = gcopy(src->point);
	if (src->order) dest->order = gcopy(src->order);
	if (src->cofactor) dest->cofactor = gcopy(src->cofactor);
	return dest;
}

point_t *point_clone(const point_t *src, point_t *dest) {
	if (src->point) dest->point = gclone(src->point);
	if (src->order) dest->order = gclone(src->order);
	if (src->cofactor) dest->cofactor = gclone(src->cofactor);
	return dest;
}

void point_free(point_t **point) {
	if (*point) {
		if ((*point)->point && isclone((*point)->point)) {
			gunclone((*point)->point);
		}
		if ((*point)->order && isclone((*point)->order)) {
			gunclone((*point)->order);
		}
		if ((*point)->cofactor && isclone((*point)->cofactor)) {
			gunclone((*point)->cofactor);
		}
		try_free(*point);
		*point = NULL;
	}
}

void points_free(point_t ***points) {
	if (*points) {
		try_free(*points);
		*points = NULL;
	}
}

void points_free_deep(point_t ***points, unsigned long npoints) {
	if (*points) {
		for (unsigned long i = 0; i < npoints; ++i) {
			point_free(&(*points)[i]);
		}
		points_free(points);
	}
}