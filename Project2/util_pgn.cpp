#include "util_pgn.h"

using namespace std;

void create_polygons_and_spikes(Arrangement_2 &arr, list<Polygon_2> &polygons, vector<Segment_2> &spikes){


	Edge_const_iterator eit;
	for (eit = arr.edges_begin(); eit != arr.edges_end(); ++eit){
		Segment_2 seg = Segment_2(eit->source()->point(),eit->target()->point());
		spikes.push_back(seg);

	}


	Arrangement_2::Face_const_iterator fit;
	Polygon_2 p;
	for (fit = arr.faces_begin(); fit != arr.faces_end();++fit){
		if(fit->has_outer_ccb()){

			Arrangement_2::Ccb_halfedge_const_circulator ccb = fit->outer_ccb();
			const Arrangement_2::Ccb_halfedge_const_circulator ccb_stop = fit->outer_ccb();
			do{
				{
					Segment_2 seg = Segment_2(ccb->source()->point(),ccb->target()->point());
					Segment_2 seg_aps = Segment_2(ccb->target()->point(),ccb->source()->point());
					p.push_back(ccb->source()->point());
					spikes.erase(std::remove(spikes.begin(), spikes.end(), seg), spikes.end());
					spikes.erase(std::remove(spikes.begin(), spikes.end(), seg_aps), spikes.end());
					ccb = ccb->next();
				}
			}
			while(ccb != ccb_stop);
		}
	}
	polygons.push_back(p);
	
}


Arrangement_2 build_pgn_Arrangement_2(Point_2* points, int size) {
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

Polygon_with_holes_2 build_pgn_Polygon_with_holes_2(Point_2* points, int size){
	Polygon_2 p;
	for (int i = 0; i < size; i++){
		p.push_back(points[i]);
	}
	Polygon_with_holes_2 pwh(p);
	print_polygon(pwh.outer_boundary(), "Environment polygon");
	return pwh;
}

Arrangement_2 find_visibility(Arrangement_2 env, Point_2 p) {
	
	Arrangement_2::Face_const_handle * face;
	CGAL::Arr_naive_point_location<Arrangement_2> pl(env);
	CGAL::Arr_point_location_result<Arrangement_2>::Type obj = pl.locate(p);
	face = boost::get<Arrangement_2::Face_const_handle>(&obj);
	// compute non regularized visibility area 
	// Define visibiliy object type that computes regularized visibility area
/*	typedef CGAL::Simple_polygon_visibility_2<Arrangement_2, CGAL::Tag_true> RSPV;
	Arrangement_2 regular_output;
	RSPV regular_visibility(env);
	regular_visibility.compute_visibility(p, *face, regular_output);
	return regular_output;
	*/
	Arrangement_2 non_regular_output;
	NSPV non_regular_visibility(env);
	non_regular_visibility.compute_visibility(p, *face, non_regular_output);
	return non_regular_output;
}

Arrangement_2 find_visibility_from_bound(Arrangement_2 env, Point_2 p) {
	
  Halfedge_const_handle he = env.halfedges_begin();
  
  bool edge_found = false;

  while (!edge_found){
    Segment_2 seg = Segment_2(he->source()->point(),he->target()->point());
	   if(seg.collinear_has_on(p)){
		   edge_found = true;
	   }
	   else{
		he++;
	   }
  }
  
  Arrangement_2 output_arr;
  TEV tev(env);
  Face_handle fh = tev.compute_visibility(p, he, output_arr);
  return output_arr;
}

Polygon_2 convert_Arrangement_2_to_Polygon_2(Arrangement_2 pol_arr2, string message, bool revert_orientation){
	Polygon_2 p;
	if (revert_orientation){
		for (Vertex_iterator eit = pol_arr2.vertices_end(); eit != pol_arr2.vertices_begin();){
			--eit;
			p.push_back(eit->point());
		}
	}
	else{
		for (Vertex_iterator eit = pol_arr2.vertices_begin(); eit != pol_arr2.vertices_end(); ++eit){
			p.push_back(eit->point());
		}
	}

	print_polygon(p, message);
	std::cout << std::endl;
	return p;
}

Point_2 find_inter_point(Polygon_with_holes_2 pwh){
	Polygon_2::Vertex_const_iterator vertex_it_bottom = pwh.outer_boundary().bottom_vertex();
	Polygon_2::Vertex_const_iterator vertex_it_begin = pwh.outer_boundary().vertices_begin();
	Polygon_2::Vertex_const_iterator vertex_it_end = pwh.outer_boundary().vertices_end();
	Point_2 p1, p2, p3, pm1, pm2;
	*--vertex_it_end;
	if (*vertex_it_bottom == *vertex_it_begin){
		p1 = *vertex_it_end;
		p2 = *vertex_it_bottom;
		p3 = *++vertex_it_bottom;
	}
	else{
		p1 = *--vertex_it_bottom;
		p2 = *++vertex_it_bottom;
		if (*vertex_it_bottom == *vertex_it_end){
			*vertex_it_bottom = *vertex_it_begin;
		}
		else{
			*++vertex_it_bottom;
		}
		p3 = *vertex_it_bottom;
	}
	pm1 = p2 + (p1 - p2) / 2.0;
	pm2 = p2 + (p3 - p2) / 2.0;
	cout << "pm1: " << pm1 << endl;
	cout << "pm2: " << pm2 << endl;
	Segment_2 inter_seg(pm1, pm2);
	//Segment_2 edge(*vertex_it_bottom, *vertex_it_bottom);
	Point_2 tmp_point = *vertex_it_bottom;
	Point_2 res;
	bool intersect = false;
	//*vertex_it = p2;
	while (*vertex_it_bottom != p1 && !intersect){
		if (*vertex_it_bottom == *vertex_it_end){
			*vertex_it_bottom = *vertex_it_begin;
		}
		else{
			*++vertex_it_bottom;
		}
		Segment_2 edge(tmp_point, *vertex_it_bottom);
		tmp_point = *vertex_it_bottom;
		cout << "edge: " << edge << endl;
		CGAL::Object obj = CGAL::intersection(inter_seg, edge);
		if (const Point_2 *inter_point = CGAL::object_cast<Point_2>(&obj)) {
			cout << "inter_point: " << *inter_point << endl;
			res = pm2 + (*inter_point - pm2) / 2.0;
			intersect = true;
		}
	}
	if (!intersect){
		res = pm2 + (pm1 - pm2) / 2.0;
	}
	return res;
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

