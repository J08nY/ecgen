
#include "subgroup.h"
#include "point.h"

OBJ(subgroup, subgroup_t, subgroup_copy, subgroup_clone)
OBJS(subgroup, subgroup_t, subgroup_copy, subgroup_clone)

subgroup_t *subgroup_copy(const subgroup_t *src, subgroup_t *dst) {
	if (src->generator) dst->generator = point_new_copy(src->generator);
	if (src->points) {
		dst->points = points_new_copy(src->points, src->npoints);
		dst->npoints = src->npoints;
	}
	return dst;
}

subgroup_t *subgroup_clone(const subgroup_t *src, subgroup_t *dst) {
	if (src->generator) dst->generator = point_new_clone(src->generator);
	if (src->points) {
		dst->points = points_new_clone(src->points, src->npoints);
		dst->npoints = src->npoints;
	}
	return dst;
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

void subgroups_free(subgroup_t ***subgroups) {
	if (*subgroups) {
		try_free(*subgroups);
		*subgroups = NULL;
	}
}

void subgroups_free_deep(subgroup_t ***subgroups, size_t num) {
	if (*subgroups) {
		for (size_t i = 0; i < num; ++i) {
			subgroup_free_deep(&(*subgroups)[i]);
		}
		subgroups_free(subgroups);
	}
}
