/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include "gen/types.h"
#include "gen/point.h"
#include "test/utils.h"
#include <criterion/criterion.h>

TestSuite(point, .init=default_setup, .fini=default_teardown);

Test(point, test_point_random) {
	GEN e = ellinit(mkvec2s(1, 3), stoi(23), -1);
	curve_t curve = {.order = stoi(27), .curve = e};
	config_t cfg = {};
	int ret = point_gen_random(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, "Point wasn't generated.");
	cr_assert_eq(curve.npoints, 1, "Incorrect number of points.");
	cr_assert_not_null(curve.points, "Points are null.");
	cr_assert(ellisoncurve(e, curve.points[0]->point), "Point not on curve.");
	cr_assert(gequal(ellorder(e, curve.points[0]->point, NULL),
					 curve.points[0]->order), "Point has wrong order set.");

	points_free_deep(&curve.points, 1);
}

Test(point, test_points_random) {
	GEN e = ellinit(mkvec2s(1, 3), stoi(23), -1);
	curve_t curve = {.order = stoi(27), .curve = e};
	config_t cfg = {};
	size_t npoints = 3;
	arg_t arg = {.args = &npoints, .nargs = 1};
	int ret = points_gen_random(&curve, &cfg, &arg);

	cr_assert_eq(ret, 1, "Points weren't generated.");
	cr_assert_eq(curve.npoints, npoints, "Incorrect number of points.");
	cr_assert_not_null(curve.points, "Points are null.");
	for (size_t i = 0; i < npoints; i++) {
		point_t *point = curve.points[i];
		cr_assert(ellisoncurve(e, point->point),
				  "Point not on curve.");
		cr_assert(gequal(ellorder(e, point->point, NULL), point->order),
				  "Point has wrong order set.");
	}

	points_free_deep(&curve.points, npoints);
}
