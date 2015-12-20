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
	string polygon_path = "polygon4.txt";
	string camera_path = "þþcamera5.txt";

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

	//Read the files.
	int polygon_size = 0;
	int camera_size = 0;
	Point_2* polygon_points = readFile(polygon_path, polygon_size);
	Point_2* camera_points = readFile(camera_path, camera_size);

	//Build the environment polygon with two datatype: Arrangement_2, Polygon_2.
	Arrangement_2 env = build_env(polygon_points, polygon_size);
	Polygon_2 env_pgn = build_polygon(env, "environment polygon");
	Arrangement_2 regular_output;
	list<Polygon_2> polygons;
	for (int i = 0; i < camera_size; i++){
		regular_output = find_visibility(env, camera_points[i]);
		polygons.push_back(build_polygon(regular_output, "view Polygon "));
	}

	//Join the seen polygons.
	Pwh_list_2 res;
	Pwh_list_2::const_iterator it;
	CGAL::join(polygons.begin(), polygons.end(), std::back_inserter(res));

	// Print the differnce.
	std::cout << "The Join polygon:" << std::endl;
	for (it = res.begin(); it != res.end(); ++it) {
		std::cout << "--> "<< endl;
		for (it = res.begin(); it != res.end(); ++it) {
			std::cout << "--> ";
			print_polygon_with_holes(*it);
		}
	}

	/*
	// Print the result polygon.
	std::cout << "The Join polygon:" << std::endl;
	print_polygon_with_holes(res);

	//Calculate the differnce between seen polygon and the original polygon. 
	Pwh_list_2 difference;
	Pwh_list_2::const_iterator it;

	CGAL::difference(env_pgn, res, std::back_inserter(difference));

	// Print the differnce.
	std::cout << "The difference polygon:" << std::endl;
	for (it = difference.begin(); it != difference.end(); ++it) {
		std::cout << "--> ";
		print_polygon_with_holes(*it);
	}

	*/
	boost::timer timer;


	double secs = timer.elapsed();
	system("pause");
	return 0;
}