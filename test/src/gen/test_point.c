/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */

#include <criterion/criterion.h>
#include "gen/point.h"
#include "math/subgroup.h"
#include "test/io.h"

TestSuite(point, .init = io_setup, .fini = io_teardown);

Test(point, test_point_clone) {
	point_t *one = point_new();
	point_t *other = point_new_clone(one);
	cr_assert_not_null(other, );
	point_clone(one, other);
	cr_assert_not_null(other, );
	point_free(&one);
	point_free(&other);
}

Test(point, test_points_clone) {
	point_t *one = point_new();
	point_t **points = points_new(1);
	points[0] = one;
	point_t **others = points_new_clone(points, 1);
	cr_assert_not_null(others, );
	points_clone(points, others, 1);
	cr_assert_not_null(others, );
	points_free_deep(&points, 1);
	points_free_deep(&others, 1);
}

// TODO: add utility functions to setup the two example curves used in these tests.

Test(point, test_point_random) {
	// curve = ellinit([1, 3], 23), order = 27
	GEN e = ellinit(mkvec2s(1, 3), stoi(23), -1);
	GEN gen = mkvec2(mkintmodu(15, 23), mkintmodu(14, 23));
	point_t gen_point = {.point = gen, .order = stoi(27), .cofactor = stoi(1)};
	subgroup_t gen_subgroup = {.generator = &gen_point};
	subgroup_t *generators[1] = {&gen_subgroup};
	curve_t curve = {.order = stoi(27), .curve = e, .generators = generators};
	int ret = point_gen_random(&curve, NULL, OFFSET_POINTS);

	cr_assert_eq(ret, 1, "Point wasn't generated.");
	cr_assert_eq(gen_subgroup.npoints, 1, "Incorrect number of points.");
	cr_assert_not_null(gen_subgroup.points, "Points are null.");
	cr_assert(ellisoncurve(e, gen_subgroup.points[0]->point), "Point not on curve.");
	cr_assert(gequal(ellorder(e, gen_subgroup.points[0]->point, NULL),
					 gen_subgroup.points[0]->order),
	          "Point has wrong order set.");

	points_free_deep(&gen_subgroup.points, gen_subgroup.npoints);
}

Test(point, test_point_random_more_gens) {
	GEN e = ellinit(mkvec2s(2,3), stoi(23), -1);
	GEN one_gen = mkvec2(mkintmodu(6, 23), mkintmodu(1,23));
	point_t *one_gen_point = point_new();
	one_gen_point->point = one_gen;
	one_gen_point->order = stoi(12);
	one_gen_point->cofactor = stoi(2);
	subgroup_t *one_subgroup = subgroup_new();
	one_subgroup->generator = one_gen_point;
	GEN two_gen = mkvec2(mkintmodu(20, 23), mkintmodu(19, 23));
	point_t *two_gen_point = point_new();
	two_gen_point->point = two_gen;
	two_gen_point->order =  stoi(6);
	two_gen_point->cofactor = stoi(4);
	subgroup_t *two_subgroup = subgroup_new();
	two_subgroup->generator = two_gen_point;
	subgroup_t **subgroups = subgroups_new(2);
	subgroups[0] = one_subgroup;
	subgroups[1] = two_subgroup;
	curve_t curve = {.order = stoi(24), .curve = e, .generators = subgroups, .ngens = 2};
	int ret = point_gen_random(&curve, NULL, OFFSET_POINTS);

	cr_assert_eq(ret, 1, "Point wasn't generated.");
	size_t generated = 0;
	for (size_t i = 0; i < 2; ++i) {
		subgroup_t *subgroup = curve.generators[i];
		if (subgroup->npoints > 0) {
			generated += subgroup->npoints;
			cr_assert_not_null(subgroup->points, "Points are null.");
			cr_assert(ellisoncurve(e, subgroup->points[0]->point), "Point not on curve.");
			cr_assert(gequal(ellorder(e, subgroup->points[0]->point, NULL),
							 subgroup->points[0]->order),
					  "Point has wrong order set.");
		}
	}
	cr_assert_eq(generated, 1, "Point wasn't saved.");

	subgroups_free_deep(&subgroups, 2);
}

Test(point, test_points_random) {
	// curve = ellinit([1, 3], 23), order = 27
	GEN e = ellinit(mkvec2s(1, 3), stoi(23), -1);
	GEN gen = mkvec2(mkintmodu(15, 23), mkintmodu(14, 23));
	point_t gen_point = {.point = gen, .order = stoi(27), .cofactor = stoi(1)};
	subgroup_t gen_subgroup = {.generator = &gen_point};
	subgroup_t *generators[1] = {&gen_subgroup};
	curve_t curve = {.order = stoi(27), .curve = e, .generators = generators, .ngens = 1};
	size_t npoints = 3;
	arg_t arg = {.args = &npoints, .nargs = 1};
	int ret = points_gen_random(&curve, &arg, OFFSET_POINTS);

	cr_assert_eq(ret, 1, "Points weren't generated.");
	cr_assert_eq(gen_subgroup.npoints, npoints, "Incorrect number of points.");
	cr_assert_not_null(gen_subgroup.points, "Points are null.");
	for (size_t i = 0; i < npoints; i++) {
		point_t *point = gen_subgroup.points[i];
		cr_assert(ellisoncurve(e, point->point), "Point not on curve.");
		cr_assert(gequal(ellorder(e, point->point, NULL), point->order),
		          "Point has wrong order set.");
	}

	points_free_deep(&gen_subgroup.points, npoints);
}

Test(point, test_points_trial) {
	// curve = ellinit([1, 3], 23), order = 27
	GEN e = ellinit(mkvec2s(1, 3), stoi(23), -1);
	GEN gen = mkvec2(mkintmodu(15, 23), mkintmodu(14, 23));
	point_t gen_point = {.point = gen, .order = stoi(27), .cofactor = stoi(1)};
	subgroup_t gen_subgroup = {.generator = &gen_point};
	subgroup_t *generators[1] = {&gen_subgroup};
	curve_t curve = {.order = stoi(27), .curve = e, .generators = generators, .ngens = 1};
	pari_ulong prime = 3;
	arg_t arg = {.args = &prime, .nargs = 1};
	int ret = points_gen_trial(&curve, &arg, OFFSET_POINTS);

	cr_assert_eq(ret, 1, "Points weren't generated.");
	cr_assert_eq(gen_subgroup.npoints, 1, "Incorrect number of points.");
	cr_assert_not_null(gen_subgroup.points, "Points are null.");
	cr_assert(ellisoncurve(e, gen_subgroup.points[0]->point), "Point not on curve.");
	cr_assert(gequal(ellorder(e, gen_subgroup.points[0]->point, NULL),
					 gen_subgroup.points[0]->order),
	          "Point has wrong order set.");
	cr_assert(gequal(gen_subgroup.points[0]->order, utoi(prime)),
	          "Point has wrong order.");

	points_free_deep(&gen_subgroup.points, 1);
}

Test(point, test_points_prime) {
	// curve = ellinit([1, 3], 23), order = 27
	GEN e = ellinit(mkvec2s(1, 3), stoi(23), -1);
	GEN gen = mkvec2(mkintmodu(15, 23), mkintmodu(14, 23));
	point_t gen_point = {.point = gen, .order = stoi(27), .cofactor = stoi(1)};
	subgroup_t gen_subgroup = {.generator = &gen_point};
	subgroup_t *generators[1] = {&gen_subgroup};
	curve_t curve = {.order = stoi(27), .curve = e, .generators = generators, .ngens = 1};
	pari_ulong prime = 3;
	int ret = points_gen_prime(&curve, NULL, OFFSET_POINTS);

	cr_assert_eq(ret, 1, "Points weren't generated.");
	cr_assert_eq(gen_subgroup.npoints, 1, "Incorrect number of points.");
	cr_assert_not_null(gen_subgroup.points, "Points are null.");
	cr_assert(ellisoncurve(e, gen_subgroup.points[0]->point), "Point not on curve.");
	cr_assert(gequal(ellorder(e, gen_subgroup.points[0]->point, NULL),
					 gen_subgroup.points[0]->order),
	          "Point has wrong order set.");
	cr_assert(gequal(gen_subgroup.points[0]->order, utoi(prime)),
	          "Point has wrong order.");

	points_free_deep(&gen_subgroup.points, 1);
}

Test(point, test_points_all) {
	// curve = ellinit([1, 3], 23), order = 27
	GEN e = ellinit(mkvec2s(1, 3), stoi(23), -1);
	GEN gen = mkvec2(mkintmodu(15, 23), mkintmodu(14, 23));
	point_t gen_point = {.point = gen, .order = stoi(27), .cofactor = stoi(1)};
	subgroup_t gen_subgroup = {.generator = &gen_point};
	subgroup_t *generators[1] = {&gen_subgroup};
	curve_t curve = {.order = stoi(27), .curve = e, .generators = generators, .ngens = 1};
	GEN orders = mkvec3s(3, 9, 27);
	size_t npoints = 3;
	int ret = points_gen_allgroups(&curve, NULL, OFFSET_POINTS);

	cr_assert_eq(ret, 1, "Points weren't generated.");
	cr_assert_eq(gen_subgroup.npoints, npoints, "Incorrect number of points.");
	cr_assert_not_null(gen_subgroup.points, "Points are null.");
	for (size_t i = 0; i < npoints; i++) {
		point_t *point = gen_subgroup.points[i];
		cr_assert(ellisoncurve(e, point->point), "Point not on curve.");
		cr_assert(gequal(ellorder(e, point->point, NULL), point->order),
		          "Point has wrong order set.");
		cr_assert(gequal(point->order, gel(orders, i + 1)),
		          "Point has wrong order.");
	}

	points_free_deep(&gen_subgroup.points, 1);
}

Test(point, test_points_nonprime) {
	// curve = ellinit([1, 3], 23), order = 27
	GEN e = ellinit(mkvec2s(1, 3), stoi(23), -1);
	GEN gen = mkvec2(mkintmodu(15, 23), mkintmodu(14, 23));
	point_t gen_point = {.point = gen, .order = stoi(27), .cofactor = stoi(1)};
	subgroup_t gen_subgroup = {.generator = &gen_point};
	subgroup_t *generators[1] = {&gen_subgroup};
	curve_t curve = {.order = stoi(27), .curve = e, .generators = generators, .ngens = 1};
	GEN orders = mkvec2s(9, 27);
	size_t npoints = 2;
	int ret = points_gen_nonprime(&curve, NULL, OFFSET_POINTS);

	cr_assert_eq(ret, 1, "Points weren't generated.");
	cr_assert_eq(gen_subgroup.npoints, npoints, "Incorrect number of points.");
	cr_assert_not_null(gen_subgroup.points, "Points are null.");
	for (size_t i = 0; i < npoints; i++) {
		point_t *point = gen_subgroup.points[i];
		cr_assert(ellisoncurve(e, point->point), "Point not on curve.");
		cr_assert(gequal(ellorder(e, point->point, NULL), point->order),
		          "Point has wrong order set.");
		cr_assert(gequal(point->order, gel(orders, i + 1)),
		          "Point has wrong order.");
	}

	points_free_deep(&gen_subgroup.points, 1);
}
