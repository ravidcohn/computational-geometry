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

	//Read the files.
	int polygon_size = 0;
	int camera_size = 0;
	Point_2* polygon_points = readFile(polygon_path, polygon_size);
	Point_2* camera_points = readFile(camera_path, camera_size);

	boost::timer timer;

	//Build the environment polygon with two datatype: Arrangement_2, Polygon_2.
	Arrangement_2 env_arr = build_pgn_Arrangement_2(polygon_points, polygon_size);
	Polygon_with_holes_2 env_pgn = build_pgn_Polygon_with_holes_2(polygon_points, polygon_size);
	Arrangement_2 non_regular_output;
	list<Polygon_2> polygons;
	vector<Segment_2> spikes;
	bool revert_orientation = true;
	for (int i = 0; i < camera_size; i++){
		switch(CGAL::bounded_side_2(polygon_points, polygon_points+polygon_size,camera_points[i], K())){
		case CGAL::ON_BOUNDED_SIDE:{
			non_regular_output = find_visibility(env_arr, camera_points[i]);
			create_polygons_and_spikes(non_regular_output, polygons, spikes);
			break;
								   }
		case CGAL::ON_BOUNDARY:{
			non_regular_output = find_visibility_from_bound(env_arr, camera_points[i]);
			create_polygons_and_spikes(non_regular_output, polygons, spikes);
			break;
							   }   
		}
		}
	cout<< "spikes: "<<'\n';
	for(std::vector<Segment_2>::const_iterator itr = spikes.begin(); itr != spikes.end(); itr++){
		cout<< itr->point(0) << " <-> "  << itr->point(1) << '\n';
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
		//Split the differnce polygon by the segments "spikes".
		Arrangement_2 arr_difference = convert_Polygon_2_to_Arrangement_2(difference);
		Polygon_2 res_pgn = intersect_Arrangement_2_with_segments(arr_difference, spikes);
		print_polygon(res_pgn, "res pgn:");
		//Find inter point in the unseen polygon.
		Point_2 res = find_inter_point(res_pgn);
		double x = CGAL::to_double(res.x());
		double y = CGAL::to_double(res.y());
		cout << "The cameras do not cover the point (" << setprecision(6) << x << "," << y << ")" << endl;
	}



	double secs = timer.elapsed();

	cout << "The time is: " << secs << endl;
	system("pause");
	return 0;
}