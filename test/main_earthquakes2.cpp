#include <iostream>
#include "../GetPot"
#include <sstream>
#include <fstream>
#include <string>
#include "simplification.hpp"

using namespace Eigen;
void print_help()
{
	using namespace std;
	std::cout<<"Di default vengono presi i vertici dal file:\nprove-mie/sphere.eval.vertices.txt\n";
	std::cout<<"I triangoli dal file:\nprove-mie/sphere.eval.triangles.txt\n";
	std::cout<<"I dati dal file:\ndati-perotto.txt\n";
	std::cout<<"Opzioni per simplify: -i N\n";
	cout<<"Dove N può essere:\n";
	cout<<"0) viene eseguito simplify come nella versione originale del codice\n";
	cout<<"1) viene aggiunto il peso sugli angoli\n";
	cout<<"Con -n si indica il numero di vertici della mesh semplificata\n";
}
unsigned count_lines(const std::string & file_name)
{
	std::ifstream file(file_name);
	unsigned i=0;
	std::string line;
	while(getline(file, line))
		++i;
	std::cout<<"righe: "<<i<<std::endl;
	return i;
}

Eigen::Matrix<int, Dynamic, 3> read_tri(const std::string & file_name)
{
	unsigned n_lines = count_lines(file_name);
	std::ifstream file(file_name);
	Eigen::Matrix<int, Dynamic, 3> ret_mat;
	ret_mat.resize(n_lines, 3);
	for(unsigned i=0; i<n_lines; ++i)
	{
		std::string line;
		getline(file, line);
		std::istringstream ss(line);
		std::string dump;
		ss>>dump;
		for(unsigned j=0; j<3; ++j)
		{
			int id_ver;
			ss>>id_ver;
			ret_mat(i, j) = id_ver-1;
		}
	}
	return ret_mat;
}

Eigen::Matrix<double, Dynamic, 3> read_ver(const std::string & file_name)
{
	unsigned n_lines = count_lines(file_name);
	std::ifstream file(file_name);
	Eigen::Matrix<double, Dynamic, 3> ret_mat;
	ret_mat.resize(n_lines, 3);
	for(unsigned i=0; i<n_lines; ++i)
	{
		std::string line;
		getline(file, line);
		std::istringstream ss(line);
		std::string dump;
		ss>>dump;
		for(unsigned j=0; j<3; ++j)
		{
			double coord;
			ss>>coord;
			ret_mat(i, j) = coord;
		}
	}
	return ret_mat;
}

Eigen::Matrix<double, Dynamic, 3> read_data(const std::string & file_name)
{
	unsigned n_lines = count_lines(file_name);
	std::ifstream file(file_name);
	Eigen::Matrix<double, Dynamic, 3> ret_mat;
	ret_mat.resize(n_lines, 3);
	for(unsigned i=0; i<n_lines; ++i)
	{
		std::string line;
		getline(file, line);
		std::istringstream ss(line);
		for(unsigned j=0; j<3; ++j)
		{
			double coord;
			ss>>coord;
			ret_mat(i, j) = coord;
		}
	}
	return ret_mat;
}

int main(int argc, char* argv[])
{
	using namespace geometry;
	GetPot gp(argc, argv);
	if(gp.search(2, "-h", "--help"))
    {
        print_help();
        return 0;
    }
    if(!gp.search(1, "-n"))
    {
    	std::cout<<"Usare -n per indicare il numero di vertici della mesh semplificata\n";
    	return 0;
    }
    unsigned n=gp.follow(1000, 1, "-n");
    unsigned N=0;
    if(gp.search(1, "-i"))
    	N = gp.follow(0, 1, "-i");

    std::string file_ver = "prove-mie/sphere.vertices.eval.txt";
    std::string file_tri = "prove-mie/sphere.triangles.eval.txt";
    std::string file_data = "prove-mie/dati-perotto.txt";
    
    
    if(N == 0)
    {
    	simplification<Triangle, MeshType::DATA, DataGeo> simp(read_ver(file_ver), read_tri(file_tri), read_data(file_data),
    										   1./3., 1./3., 1./3., 0., 100.0, false);
    	simp.simplify(n, true, "earthquakes.txt");
    	 simp.getCPointerToMeshOperator()->print_qoi("qoi.txt");
    }
    if(N == 1)
    {
    	simplification<Triangle, MeshType::DATA, DataGeo> simp(read_ver(file_ver), read_tri(file_tri), read_data(file_data),
    										   0.25, 0.25, 0.25, 0.25, 100.0, false);
    	simp.simplify(n, true, "earthquakes.txt");
    	 simp.getCPointerToMeshOperator()->print_qoi("qoi.txt");
    }
   
    return 0;
}