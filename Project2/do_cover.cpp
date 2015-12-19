#include <boost/timer.hpp>

#include <CGAL/Gps_circle_segment_traits_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Lazy_exact_nt.h>
#include <list>
#include <cstdlib>
#include <cmath>

#include "util_pgn.h"
#include "util_parse.h"

typedef CGAL::Polygon_with_holes_2<Kernel>                Polygon_with_holes_2;

using namespace std;

int main(int argc, char* argv[])
{
	string polygon_path = "þþpolygon4.txt";
	string camera_path = "þþcamera4.txt";

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

	int polygon_size = num_of_points(polygon_path);
	int camera_size = num_of_points(camera_path);
	Point_2* polygon_points = readFile(polygon_path);
	Point_2* camera_points = readFile(camera_path);

	Arrangement_2 env = build_env(polygon_points, polygon_size);
	Arrangement_2 regular_output;
	std::list<Polygon_2> polygons;
	for (int i = 0; i < camera_size; i++){
		regular_output = find_visibility(env, camera_points[i]);
		polygons.push_back(build_polygon(regular_output));
	}
	std::list<Polygon_with_holes_2> res;
	CGAL::join(polygons.begin(), polygons.end(), std::back_inserter(res));
	// Print the result.
	std::copy(res.begin(), res.end(),
		std::ostream_iterator<Polygon_with_holes_2>(std::cout, "\n"));
	std::cout << std::endl;

	boost::timer timer;


	double secs = timer.elapsed();
	system("pause");
	return 0;
}