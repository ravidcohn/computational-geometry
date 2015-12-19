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

// This is the content of the .h file, which is where the declarations go
Arrangement_2 build_env(Point_2* points, int size);	// function prototype for add.h -- don't forget the semicolon!
Arrangement_2 find_visibility(Arrangement_2 env, Point_2 p); 
Polygon_2 build_polygon(Arrangement_2 pol_arr2);


// This is the end of the header guard
#endif