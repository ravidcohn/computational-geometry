#include <boost/timer.hpp>
#include <CGAL/Gps_circle_segment_traits_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Lazy_exact_nt.h>
#include <list>
#include <cstdlib>
#include <cmath>

#include "util_pgn.h"
#include "util_parse.h"



typedef std::list<Polygon_with_holes_2>                   Pwh_list_2;

using namespace std;


int main(int argc, char* argv[])
{
	//Default path for the inputs files.
	string polygon_path = "polygon.txt";
	string camera_path = "camera.txt";
	//Gets the files path from user.
	switch (argc){
		case 2:{
			polygon_path = argv[1];
			break;
		}
		case 3:{
			polygon_path = argv[1];
			camera_path = argv[2];
			break;
		}
	}
	boost::timer timer;

	//Read the files.
	int polygon_size = 0;
	int camera_size = 0;
	Point_2* polygon_points = readFile(polygon_path, polygon_size);
	Point_2* camera_points = readFile(camera_path, camera_size);

	//Build the environment polygon with two datatype: Arrangement_2, Polygon_2.
	Arrangement_2 env_arr = build_pgn_Arrangement_2(polygon_points, polygon_size);
	Polygon_with_holes_2 env_pgn = build_pgn_Polygon_with_holes_2(polygon_points, polygon_size);
	Arrangement_2 regular_output;
	list<Polygon_2> polygons;
	bool revert_orientation = true;
	for (int i = 0; i < camera_size; i++){
		regular_output = find_visibility(env_arr, camera_points[i]);
		polygons.push_back(convert_Arrangement_2_to_Polygon_2(regular_output, "view Polygon ", revert_orientation));
	}

	//Join the seen polygons.
	Pwh_list_2 join_pgn;
	Pwh_list_2::const_iterator it_join_pgn;
	CGAL::join(polygons.begin(), polygons.end(), std::back_inserter(join_pgn));

	// Print the joint polygon.
	std::cout << "The Join polygon:" << std::endl;
	for (it_join_pgn = join_pgn.begin(); it_join_pgn != join_pgn.end(); ++it_join_pgn) {
			std::cout << "--> ";
			print_polygon_with_holes(*it_join_pgn);
	}

	//Calculate the differnce between seen polygon and the original polygon.
	Pwh_list_2 difference_list;
	Polygon_with_holes_2 difference = env_pgn;
	bool cover = false;
	for (it_join_pgn = join_pgn.begin(); it_join_pgn != join_pgn.end(); ++it_join_pgn) {
		CGAL::difference(difference, *it_join_pgn, std::back_inserter(difference_list));
		if (difference_list.size() > 0){
			difference = difference_list.front();
		}
		else{
			cover = true;
			break;
		}
	}

	//Print result.
	if (cover){
		cout << "The cameras cover the polygon!" << endl;
	}
	else{
		Polygon_2 res = difference.outer_boundary();
		Point_2 p = *res.vertices_begin();
		double x = CGAL::to_double(p.x());
		double y = CGAL::to_double(p.y());
		cout << "The cameras do not cover the point (" << setprecision(6) << x << "," << y << ")" << endl;
		print_polygon_with_holes(difference);
	}


	/*
	//Build inverce polygon.
	std::vector<Polygon_2>  holes;
	int index = 0;
	for (it_join_pgn = join_pgn.begin(); it_join_pgn != join_pgn.end(); ++it_join_pgn) {
		Polygon_with_holes_2 q = *it_join_pgn;
		holes.push_back(q.outer_boundary());
		index++;
	}
	Polygon_with_holes_2 inv_pwh(env_pgn.outer_boundary(), holes.begin(), holes.end());
	print_polygon_with_holes(inv_pwh);

	//Calculate the intersect between inv_pwh and the env_pgn.
	Pwh_list_2 intersect;
	CGAL::intersection(env_pgn, inv_pwh, std::back_inserter(intersect));
	print_polygon_with_holes(intersect.front());

	//Calculate the differnce between seen polygon and the original polygon.
	Pwh_list_2 difference;
	Pwh_list_2::const_iterator it_difference_pgn;
	CGAL::difference(difference.front(), *it_join_pgn, std::back_inserter(difference));


	// Print the differnce.
	std::cout << "The difference polygon:" << std::endl;
	for (it = difference.begin(); it != difference.end(); ++it) {
		std::cout << "--> ";
		print_polygon_with_holes(*it);
	}

	*/


	double secs = timer.elapsed();

	cout << "The time is: " << secs << endl;
	system("pause");
	return 0;
}