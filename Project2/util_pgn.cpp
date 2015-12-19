#include "util_pgn.h"

Arrangement_2 build_env(Point_2* points, int size) {
	//create environment
	std::vector<Segment_2> segments;
	for (int i = 0; i < size;i++){
		if (i < size - 1){
			segments.push_back(Segment_2(points[i], points[i+1]));
		}
		else{
			segments.push_back(Segment_2(points[i], points[0]));
		}
	}
	Arrangement_2 env;
	CGAL::insert_non_intersecting_curves(env, segments.begin(), segments.end());
	return env;
}

Arrangement_2 find_visibility(Arrangement_2 env, Point_2 p) {
	Arrangement_2::Face_const_handle * face;
	CGAL::Arr_naive_point_location<Arrangement_2> pl(env);
	CGAL::Arr_point_location_result<Arrangement_2>::Type obj = pl.locate(p);
	face = boost::get<Arrangement_2::Face_const_handle>(&obj);
	// compute non regularized visibility area 
	// Define visibiliy object type that computes regularized visibility area
	typedef CGAL::Simple_polygon_visibility_2<Arrangement_2, CGAL::Tag_true> RSPV;
	Arrangement_2 regular_output;
	RSPV regular_visibility(env);
	regular_visibility.compute_visibility(p, *face, regular_output);
	return regular_output;
}

