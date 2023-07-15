#include "simplification.hpp"
#include <fstream>
#include <sstream>

/*
void read_triangles(Eigen::Matrix<int, 6190, 3> & triangles, const std::string & file_name)
{
	std::ifstream file(file_name);
	std::string line;
	unsigned i=0;
	while(getline(file, line))
	{
		std::istringstream ss(line);
		std::string n;
		ss>>n;
		for(int j=0; j<3; ++j)
		{
			ss>>n;
			triangles(i, j) = std::stoi(n)-1;
		}
		++i;
	}
}

void read_vertices(Eigen::Matrix<double, 3097, 3> & vertices, const std::string & file_name)
{
	std::ifstream file(file_name);
	std::string line;
	unsigned i=0;
	while(getline(file, line))
	{
		std::istringstream ss(line);
		std::string coord;
		ss>>coord;
		for(int j=0; j<3; ++j)
		{
			ss>>coord;
			vertices(i, j) = std::stod(coord);
		}
		++i;
	}
}
void read_data(Eigen::Matrix<double, 4000, 3> & data, int j, const std::string & file_name)
{
	std::ifstream file(file_name);
	std::string line;
	unsigned i=0;
	while(getline(file, line) && i<4000)
	{
		std::istringstream ss(line);
		std::string coord;
		ss>>coord;
		data(i, j) = std::stod(coord);
		++i;
	}
}*/

Eigen::MatrixXd read_data(const Eigen::Matrix<double,5040 ,3 > &nodes, double tol)
{
	using namespace geometry;
	std::ifstream file_x("x.inp");
	std::ifstream file_y("y.inp");
	std::ifstream file_z("z.inp");
	std::string x, y, z;
	unsigned valid_data = 0;
	std::vector<point> valid_data_vec;
	unsigned k =0;
	while( getline(file_x, x) && k<3500 )
	{
		getline(file_y, y);
		getline(file_z, z);
		point data_point(std::stod(x), std::stod(y), std::stod(z));
		for(unsigned i =0; i< 5040; ++i)
		{
			point node_point(nodes(i, 0), nodes(i, 1), nodes(i, 2));
			if( (data_point-node_point).norm2() < tol )
			{
				valid_data++;
				valid_data_vec.push_back(data_point);
				break;
			} 
		}
		++k;
	}

	Eigen::MatrixXd ret_mat;
	ret_mat.resize(valid_data, 3);

	for(unsigned i =0 ; i<valid_data; ++i)
	{
		ret_mat(i, 0) = valid_data_vec[i][0];
		ret_mat(i, 1) = valid_data_vec[i][1];
		ret_mat(i, 2) = valid_data_vec[i][2];
	}
	return ret_mat;
}

void read_mesh(Eigen::Matrix<double, 5040, 3> & vertices, Eigen::Matrix<int, 10076, 3> & triangles)
{
	std::ifstream file("sfera.inp");
	std::string line;
	getline(file, line);
	for(unsigned i=0; i<5040; ++i)
	{
		getline(file, line);
		std::istringstream ss(line);
		double coord;
		ss>>coord;
		for(unsigned j=0; j<3; ++j)
		{
			ss>>coord;
			vertices(i, j) = coord;
		}
	}
	
	for(unsigned i=0; i<10076; ++i)
	{
		getline(file, line);
		std::istringstream ss(line);
		int n_ver;
		for(unsigned j=0; j<3; ++j)
			ss>>n_ver;
		for(unsigned j=0; j<3; ++j)
		{
			ss>>n_ver;
			triangles(i, j) = n_ver-1;
		}
	}

}

void print(const Eigen::MatrixXd m)
{
	std::cout<<"righe: "<<m.rows()<<", colonne: "<<m.cols()<<"\n";
}

int main()
{
	using namespace geometry;
	/*
	Vengono presi i dati da tre file esterni. Los stesso per la mesh. 
	*/
	Eigen::Matrix<int, 10076, 3> triangles;
	Eigen::Matrix<double, 5040, 3> vertices;
	std::cout<<"\nReading the mesh...\n";
	read_mesh(vertices, triangles);
	std::cout<<"Reading the data points..\n";
	Eigen::Matrix<double, Dynamic, 3> data(read_data(vertices, 0.1));
	print(data);
	simplification<Triangle, MeshType::DATA, DataGeo> simplifier(vertices, triangles, vertices);
	simplifier.simplify(350, true, "earthquakes.txt");

	return 0;

}