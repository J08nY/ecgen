
#include "point.h"
#include "misc/types.h"
#include "util/memory.h"

point_t *point_new(void) { return try_calloc(sizeof(point_t)); }

point_t *point_copy(const point_t *src, point_t *dest) {
	if (src->point) dest->point = gcopy(src->point);
	if (src->order) dest->order = gcopy(src->order);
	if (src->cofactor) dest->cofactor = gcopy(src->cofactor);
	return dest;
}

point_t *point_new_copy(const point_t *src) {
	point_t *result = point_new();
	return point_copy(src, result);
}

point_t *point_clone(const point_t *src, point_t *dest) {
	if (src->point) dest->point = gclone(src->point);
	if (src->order) dest->order = gclone(src->order);
	if (src->cofactor) dest->cofactor = gclone(src->cofactor);
	return dest;
}

point_t *point_new_clone(const point_t *src) {
	point_t *result = point_new();
	return point_clone(src, result);
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

point_t **points_new(unsigned long num) {
	return try_calloc(num * sizeof(point_t *));
}

point_t **points_copy(point_t **const src, point_t **dest, unsigned long num) {
	for (unsigned long i = 0; i < num; ++i) {
		dest[i] = point_new_copy(src[i]);
	}
	return dest;
}

point_t **points_new_copy(point_t **const src, unsigned long num) {
	point_t **result = points_new(num);
	return points_copy(src, result, num);
}

point_t **points_clone(point_t **const src, point_t **dest, unsigned long num) {
	for (unsigned long i = 0; i < num; ++i) {
		dest[i] = point_new_clone(src[i]);
	}
	return dest;
}

point_t **points_new_clone(point_t **const src, unsigned long num) {
	point_t **result = points_new(num);
	return points_clone(src, result, num);
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