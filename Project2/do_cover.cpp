#include <boost/timer.hpp>
//test github
#include <CGAL/Gps_circle_segment_traits_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Lazy_exact_nt.h>
#include <list>
#include <cstdlib>
#include <cmath>

#include "util_pgn.h"
#include "util_parse.h"

typedef CGAL::General_polygon_set_2<Traits_2>             Polygon_set_2;

using namespace std;

int main(int argc, char* argv[])
{
	string polygon_path = "polygon.txt";
	string camera_path = "camera.txt";

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

	int* polygon_size;
	int* camera_size;
	Point_2* polygon_points = readFile(polygon_path, &polygon_size);
	Point_2* camera_points = readFile(camera_path, &camera_size);

	Arrangement_2 env = build_env(polygon_points, 3);
	Arrangement_2 regular_output;
	std::list<Arrangement_2> polygons;
	for (int i = 0; i < 1; i++){
		regular_output = find_visibility(env, camera_points[i]);
		polygons.push_back(regular_output);

	}
	//std::list<Polygon_with_holes_2> res;
	//CGAL::join(polygons.begin(), polygons.end(), std::back_inserter(res));

	boost::timer timer;


	double secs = timer.elapsed();
	system("pause");
	return 0;
}