
#include "curve.h"
#include "gen/seed.h"
#include "obj/subgroup.h"

OBJ(curve, curve_t, curve_copy, curve_clone)

curve_t *curve_copy(const curve_t *src, curve_t *dest) {
	if (src->seed) dest->seed = seed_new_copy(src->seed);
	if (src->field) dest->field = gcopy(src->field);
	if (src->a) dest->a = gcopy(src->a);
	if (src->b) dest->b = gcopy(src->b);
	if (src->curve) dest->curve = gcopy(src->curve);
	if (src->order) dest->order = gcopy(src->order);
	if (src->generators) {
		dest->generators = subgroups_new_copy(src->generators, src->ngens);
		dest->ngens = src->ngens;
	}
	return dest;
}

curve_t *curve_clone(const curve_t *src, curve_t *dest) {
	if (src->seed) dest->seed = seed_new_clone(src->seed);
	if (src->field) dest->field = gclone(src->field);
	if (src->a) dest->a = gclone(src->a);
	if (src->b) dest->b = gclone(src->b);
	if (src->curve) dest->curve = gclone(src->curve);
	if (src->order) dest->order = gclone(src->order);
	if (src->generators) {
		dest->generators = subgroups_new_clone(src->generators, src->ngens);
		dest->ngens = src->ngens;
	}
	return dest;
}

void curve_free(curve_t **curve) {
	if (*curve) {
		seed_free(&(*curve)->seed);
		subgroups_free_deep(&(*curve)->generators, (*curve)->ngens);

		if ((*curve)->curve) {
			// TODO, this is possibly dangerous...
			obj_free((*curve)->curve);
			if (isclone((*curve)->curve)) {
				gunclone((*curve)->curve);
			}
		}

		if ((*curve)->field && isclone((*curve)->field)) {
			gunclone((*curve)->field);
		}
		if ((*curve)->a && isclone((*curve)->a)) {
			gunclone((*curve)->a);
		}
		if ((*curve)->b && isclone((*curve)->b)) {
			gunclone((*curve)->b);
		}
		if ((*curve)->order && isclone((*curve)->order)) {
			gunclone((*curve)->order);
		}

		try_free(*curve);
		*curve = NULL;
	}
}