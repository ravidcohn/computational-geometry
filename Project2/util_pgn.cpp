#include "util_pgn.h"

using namespace std;

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

Polygon_2 build_polygon(Arrangement_2 pol_arr2, string message){
	Polygon_2 p;
	//for (Vertex_iterator eit = pol_arr2.vertices_begin(); eit != pol_arr2.vertices_end(); ++eit){
	for (Vertex_iterator eit = pol_arr2.vertices_end(); eit != pol_arr2.vertices_begin(); ){
		--eit;
		//std::cout << eit->point() << std::endl;
		p.push_back(eit->point());
	}

	print_polygon(p, message);
	std::cout << std::endl;
	return p;
}

template<class Kernel, class Container>
void print_polygon(const CGAL::Polygon_2<Kernel, Container>& P, string message)
{
	typename CGAL::Polygon_2<Kernel, Container>::Vertex_const_iterator vit;
	std::cout << message << "[ " << P.size() << " vertices:";
	for (vit = P.vertices_begin(); vit != P.vertices_end(); ++vit)
		std::cout << " (" << *vit << ')';
	std::cout << " ]" << std::endl;
}

template<class Kernel, class Container>
void print_polygon_with_holes(const CGAL::Polygon_with_holes_2<Kernel, Container> pwh)
{
	if (!pwh.is_unbounded()) {
		std::cout << "{ Outer boundary = ";
		print_polygon(pwh.outer_boundary(),"");
	}
	else
		std::cout << "{ Unbounded polygon." << std::endl;
	typename CGAL::Polygon_with_holes_2<Kernel, Container>::Hole_const_iterator hit;
	unsigned int k = 1;
	std::cout << " " << pwh.number_of_holes() << " holes:" << std::endl;
	for (hit = pwh.holes_begin(); hit != pwh.holes_end(); ++hit, ++k) {
		std::cout << " Hole #" << k << " = ";
		print_polygon(*hit,"");
	}
	std::cout << " }" << std::endl;
}