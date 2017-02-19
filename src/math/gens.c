
#include "gens.h"
#include "point.h"

int gens_init(curve_t *curve, config_t *config, arg_t *args) {
	// TODO stack code!!!
	GEN generators = ellff_get_gens(curve->curve);
	long len = glength(generators);
	curve->generators = points_new((size_t)len);
	curve->ngens = (size_t)len;

	for (long i = 1; i <= len; ++i) {
		point_t *p = point_new();
		p->point = gel(generators, i);
		p->order = ellorder(curve->curve, p->point, NULL);
		p->cofactor = divii(curve->order, p->order);
		curve->generators[i - 1] = p;
	}

	return 1;
}
