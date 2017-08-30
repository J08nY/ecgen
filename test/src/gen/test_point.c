/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <criterion/criterion.h>
#include "gen/point.h"
#include "test/utils.h"

TestSuite(point, .init = default_setup, .fini = default_teardown);

Test(point, test_point_random) {
	// curve = ellinit([1, 3], 23), order = 27
	GEN e = ellinit(mkvec2s(1, 3), stoi(23), -1);
	curve_t curve = {.order = stoi(27), .curve = e};
	config_t cfg = {};
	int ret = point_gen_random(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, "Point wasn't generated.");
	cr_assert_eq(curve.npoints, 1, "Incorrect number of points.");
	cr_assert_not_null(curve.points, "Points are null.");
	cr_assert(ellisoncurve(e, curve.points[0]->point), "Point not on curve.");
	cr_assert(gequal(ellorder(e, curve.points[0]->point, NULL),
	                 curve.points[0]->order),
	          "Point has wrong order set.");

	points_free_deep(&curve.points, 1);
}

Test(point, test_points_random) {
	// curve = ellinit([1, 3], 23), order = 27
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
		cr_assert(ellisoncurve(e, point->point), "Point not on curve.");
		cr_assert(gequal(ellorder(e, point->point, NULL), point->order),
		          "Point has wrong order set.");
	}

	points_free_deep(&curve.points, npoints);
}

Test(point, test_points_trial) {
	// curve = ellinit([1, 3], 23), order = 27
	GEN e = ellinit(mkvec2s(1, 3), stoi(23), -1);
	point_t **gens = points_new(1);
	gens[0] = point_new();
	gens[0]->order = stoi(27);
	gens[0]->cofactor = stoi(1);
	gens[0]->point = mkvec2(mkintmodu(15, 23), mkintmodu(14, 23));
	curve_t curve = {
	    .order = stoi(27), .curve = e, .ngens = 1, .generators = gens};
	config_t cfg = {};
	pari_ulong prime = 3;
	arg_t arg = {.args = &prime, .nargs = 1};
	int ret = points_gen_trial(&curve, &cfg, &arg);

	cr_assert_eq(ret, 1, "Points weren't generated.");
	cr_assert_eq(curve.npoints, 1, "Incorrect number of points.");
	cr_assert_not_null(curve.points, "Points are null.");
	cr_assert(ellisoncurve(e, curve.points[0]->point), "Point not on curve.");
	cr_assert(gequal(ellorder(e, curve.points[0]->point, NULL),
	                 curve.points[0]->order),
	          "Point has wrong order set.");
	cr_assert(gequal(curve.points[0]->order, utoi(prime)),
	          "Point has wrong order.");

	points_free_deep(&curve.points, 1);
	points_free_deep(&curve.generators, 1);
}

Test(point, test_points_prime) {
	// curve = ellinit([1, 3], 23), order = 27
	GEN e = ellinit(mkvec2s(1, 3), stoi(23), -1);
	point_t **gens = points_new(1);
	gens[0] = point_new();
	gens[0]->order = stoi(27);
	gens[0]->cofactor = stoi(1);
	gens[0]->point = mkvec2(mkintmodu(15, 23), mkintmodu(14, 23));
	curve_t curve = {
	    .order = stoi(27), .curve = e, .ngens = 1, .generators = gens};
	config_t cfg = {};
	pari_ulong prime = 3;
	int ret = points_gen_prime(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, "Points weren't generated.");
	cr_assert_eq(curve.npoints, 1, "Incorrect number of points.");
	cr_assert_not_null(curve.points, "Points are null.");
	cr_assert(ellisoncurve(e, curve.points[0]->point), "Point not on curve.");
	cr_assert(gequal(ellorder(e, curve.points[0]->point, NULL),
	                 curve.points[0]->order),
	          "Point has wrong order set.");
	cr_assert(gequal(curve.points[0]->order, utoi(prime)),
	          "Point has wrong order.");

	points_free_deep(&curve.points, 1);
	points_free_deep(&curve.generators, 1);
}

Test(point, test_points_all) {
	// curve = ellinit([1, 3], 23), order = 27
	GEN e = ellinit(mkvec2s(1, 3), stoi(23), -1);
	point_t **gens = points_new(1);
	gens[0] = point_new();
	gens[0]->order = stoi(27);
	gens[0]->cofactor = stoi(1);
	gens[0]->point = mkvec2(mkintmodu(15, 23), mkintmodu(14, 23));
	curve_t curve = {
	    .order = stoi(27), .curve = e, .ngens = 1, .generators = gens};
	config_t cfg = {};
	GEN orders = mkvec3s(3, 9, 27);
	size_t npoints = 3;
	int ret = points_gen_allgroups(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, "Points weren't generated.");
	cr_assert_eq(curve.npoints, npoints, "Incorrect number of points.");
	cr_assert_not_null(curve.points, "Points are null.");
	for (size_t i = 0; i < npoints; i++) {
		point_t *point = curve.points[i];
		cr_assert(ellisoncurve(e, point->point), "Point not on curve.");
		cr_assert(gequal(ellorder(e, point->point, NULL), point->order),
		          "Point has wrong order set.");
		cr_assert(gequal(point->order, gel(orders, i + 1)),
		          "Point has wrong order.");
	}

	points_free_deep(&curve.points, 1);
	points_free_deep(&curve.generators, 1);
}

Test(point, test_points_nonprime) {
	// curve = ellinit([1, 3], 23), order = 27
	GEN e = ellinit(mkvec2s(1, 3), stoi(23), -1);
	point_t **gens = points_new(1);
	gens[0] = point_new();
	gens[0]->order = stoi(27);
	gens[0]->cofactor = stoi(1);
	gens[0]->point = mkvec2(mkintmodu(15, 23), mkintmodu(14, 23));
	curve_t curve = {
	    .order = stoi(27), .curve = e, .ngens = 1, .generators = gens};
	config_t cfg = {};
	GEN orders = mkvec2s(9, 27);
	size_t npoints = 2;
	int ret = points_gen_nonprime(&curve, &cfg, NULL);

	cr_assert_eq(ret, 1, "Points weren't generated.");
	cr_assert_eq(curve.npoints, npoints, "Incorrect number of points.");
	cr_assert_not_null(curve.points, "Points are null.");
	for (size_t i = 0; i < npoints; i++) {
		point_t *point = curve.points[i];
		cr_assert(ellisoncurve(e, point->point), "Point not on curve.");
		cr_assert(gequal(ellorder(e, point->point, NULL), point->order),
		          "Point has wrong order set.");
		cr_assert(gequal(point->order, gel(orders, i + 1)),
		          "Point has wrong order.");
	}

	points_free_deep(&curve.points, 1);
	points_free_deep(&curve.generators, 1);
}
