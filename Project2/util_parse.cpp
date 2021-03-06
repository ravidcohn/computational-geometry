#include "util_parse.h"

Point_2 parse_line(string line){
	Point_2 p;
	string::size_type sz;
	double x, y;
	size_t pos = line.find(" ");
	string str_x = line.substr(0, pos);
	string str_y = line.substr(pos + 1);
	pos = str_x.find("/");
	if (pos != string::npos) {
		x = stod(str_x.substr(0, pos), &sz) / stod(str_x.substr(pos + 1), &sz);
	}
	else{
		x = stod(str_x, &sz);
	}

	pos = str_y.find("/");
	if (pos != string::npos) {
		y = stod(str_y.substr(0, pos), &sz) / stod(str_y.substr(pos + 1), &sz);
	}
	else{
		y = stod(str_y, &sz);
	}
	p = Point_2(x, y);
	return p;
}

Point_2* readFile(const string filename, int& size){
	cout<< "file name is: "<<filename<<'\n';
	ifstream file(filename);
	if (file.is_open())
	{
		string line;
		getline(file, line);
		string::size_type sz;
		size = stoi(line, &sz);
		int count = 0;
		Point_2* data = new Point_2[size];
		//while (getline(file, line))
		for (int i = 0; i < size;i++)
		{
			getline(file, line);
			data[count++] = parse_line(line);
		}
		file.close();
		return data;
	}
	 std::ifstream fin(filename);
   if(!fin)
      perror ( "Stream Failed to open because: " );

	return NULL;
}