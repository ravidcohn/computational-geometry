// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef UTIL_PGN_H
#define UTIL_PGN_H
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Simple_polygon_visibility_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arr_naive_point_location.h>
#include <CGAL/Polygon_2.h>
#include <istream>
#include <vector>

#include <CGAL/Gps_circle_segment_traits_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Lazy_exact_nt.h>
#include <list>
#include <cstdlib>
#include <cmath>

#include "util_parse.h"


typedef CGAL::Exact_predicates_exact_constructions_kernel               Kernel;
typedef Kernel::Point_2                                                 Point_2;
typedef Kernel::Segment_2                                               Segment_2;
typedef CGAL::Arr_segment_traits_2<Kernel>                              Traits_2;
typedef CGAL::Arrangement_2<Traits_2>                                   Arrangement_2;
typedef Arrangement_2::Face_handle                                      Face_handle;
typedef Arrangement_2::Edge_const_iterator                              Edge_const_iterator;
typedef Arrangement_2::Ccb_halfedge_circulator							Ccb_halfedge_circulator;

typedef Arrangement_2::Vertex_iterator									Vertex_iterator;
typedef CGAL::Polygon_2<Kernel>										    Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel>								Polygon_with_holes_2;

// This is the content of the .h file, which is where the declarations go
Arrangement_2 build_env(Point_2* points, int size);	// function prototype for add.h -- don't forget the semicolon!
Arrangement_2 find_visibility(Arrangement_2 env, Point_2 p); 
Polygon_2 build_polygon(Arrangement_2 pol_arr2, string message);
template<class Kernel, class Container>
void print_polygon(const CGAL::Polygon_2<Kernel, Container>& P, string message);

template<class Kernel, class Container> 
void print_polygon_with_holes(const CGAL::Polygon_with_holes_2<Kernel, Container> & pwh)
{
	
	string msg ="ccddc";
	if (!pwh.is_unbounded()) {
		std::cout << "{ Outer boundary = ";
		print_polygon(pwh.outer_boundary(),msg);
	}
	else
		std::cout << "{ Unbounded polygon." << std::endl;
	typename CGAL::Polygon_with_holes_2<Kernel, Container>::Hole_const_iterator hit;
	unsigned int k = 1;
	std::cout << " " << pwh.number_of_holes() << " holes:" << std::endl;
	for (hit = pwh.holes_begin(); hit != pwh.holes_end(); ++hit, ++k) {
		std::cout << " Hole #" << k << " = ";
		print_polygon(*hit,msg);
	}
	std::cout << " }" << std::endl;
	
}

// This is the end of the header guard
#endif