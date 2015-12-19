#ifndef UTIL_PARSE_H
#define UTIL_PARSE_H
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <fstream>
#include <string>
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point_2;

using namespace std;

Point_2 parse_line(string line);
Point_2* readFile(const string filename);
int num_of_points(const string filename);

#endif