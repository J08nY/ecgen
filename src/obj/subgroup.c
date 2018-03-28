
#include "subgroup.h"
#include "point.h"
#include "util/memory.h"

subgroup_t *subgroup_new(void) { return try_calloc(sizeof(subgroup_t)); }

subgroup_t *subgroup_copy(const subgroup_t *src, subgroup_t *dst) {
	if (src->generator) dst->generator = point_new_copy(src->generator);
	if (src->points) {
		dst->points = points_new_copy(src->points, src->npoints);
		dst->npoints = src->npoints;
	}
	return dst;
}

subgroup_t *subgroup_new_copy(const subgroup_t *src) {
	subgroup_t *result = subgroup_new();
	return subgroup_copy(src, result);
}

subgroup_t *subgroup_clone(const subgroup_t *src, subgroup_t *dst) {
	if (src->generator) dst->generator = point_new_clone(src->generator);
	if (src->points) {
		dst->points = points_new_clone(src->points, src->npoints);
		dst->npoints = src->npoints;
	}
	return dst;
}

subgroup_t *subgroup_new_clone(const subgroup_t *src) {
	subgroup_t *result = subgroup_new();
	return subgroup_clone(src, result);
}

void subgroup_free(subgroup_t **subgroup) {
	if (*subgroup) {
		if ((*subgroup)->generator) {
			point_free(&(*subgroup)->generator);
		}
		try_free(*subgroup);
		*subgroup = NULL;
	}
}

void subgroup_free_deep(subgroup_t **subgroup) {
	if (*subgroup) {
		points_free_deep(&(*subgroup)->points, (*subgroup)->npoints);
		subgroup_free(subgroup);
	}
}

subgroup_t **subgroups_new(size_t num) {
	return try_calloc(num * sizeof(subgroup_t *));
}

subgroup_t **subgroups_copy(subgroup_t **const src, subgroup_t **dest,
                            size_t num) {
	for (size_t i = 0; i < num; ++i) {
		dest[i] = subgroup_new_copy(src[i]);
	}
	return dest;
}

subgroup_t **subgroups_new_copy(subgroup_t **const src, size_t num) {
	subgroup_t **result = subgroups_new(num);
	return subgroups_copy(src, result, num);
}

subgroup_t **subgroups_clone(subgroup_t **const src, subgroup_t **dest,
                             size_t num) {
	for (size_t i = 0; i < num; ++i) {
		dest[i] = subgroup_new_clone(src[i]);
	}
	return dest;
}

subgroup_t **subgroups_new_clone(subgroup_t **const src, size_t num) {
	subgroup_t **result = subgroups_new(num);
	return subgroups_clone(src, result, num);
}

void subgroups_free(subgroup_t ***subgroups) {
	if (*subgroups) {
		try_free(*subgroups);
		*subgroups = NULL;
	}
}

void subgroups_free_deep(subgroup_t ***subgroups, size_t num) {
	if (*subgroups) {
		for (size_t i = 0; i < num; ++i) {
			subgroup_free(&(*subgroups)[i]);
		}
		subgroups_free(subgroups);
	}
}
