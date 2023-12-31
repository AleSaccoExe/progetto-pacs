#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <GetPot>

void print_help()
{
    std::cout<<"Print help\n";
}

int main(int argc, char ** argv)
{
    if(argc == 1)
    {
        std::cout<<"Inserire un comando\n";
        return 0;
    }
    GetPot gp(argc, argv);
    if(gp.search(2, "-h", "--help"))
    {
        print_help();
        return 0;
    }
    if(!gp.search(2, "-t", "--type")){
        std::cout<<"Bisogna inserire -t o --type\n";
        return 0;
    }
    std::string type = gp.follow("g", 2, "-t", "--type");
    std::string file_dati = gp.follow("dati.txt", 2, "-f", "--file");
    std::string file_output = gp.follow("dati-pronti.txt", 2, "-o", "--output");

    std::ofstream gnuplot_script("gnuplot_script");
    gnuplot_script << "set hidden3d\n";
    gnuplot_script << "splot \"" << file_output << "\" with lines\n";
    gnuplot_script<<"pause -1";
    gnuplot_script.close();

    std::vector<std::array<double, 3>> vertici;
    std::vector<std::array<unsigned, 3>> triangoli;

    std::ifstream file(file_dati);
    std::ofstream dump_file(file_output);

    std::string line;
    getline(file, line);
    std::string s_num_elem,s_num_ver, num_data;
    std::istringstream ss(line);
    unsigned num_elem, num_ver;
    ss>>num_ver;
    ss>>num_elem;
    std::cout<<"numero di vertici: "<<num_ver<<", numero elementi: "<<num_elem<<std::endl;
    dump_file<<num_elem<<std::endl;
    // unsigned num_ver = std::stoi(s_num_ver);
   
    // unsigned num_elem = std::stoi(s_num_elem);
 

    vertici.reserve(num_ver);
    
    for(unsigned i = 0; i<num_ver ; ++i)
    {
        getline(file, line);
        std::string coordinata;
        std::istringstream ss(line);
        ss>>coordinata;
        vertici.push_back({0., 0., 0.});
        for(unsigned k = 0; k<3; ++k){
            ss>>coordinata;
            vertici[i][k] = std::stod(coordinata);
        }

    }

    triangoli.reserve(num_elem);
    for(unsigned i = 0; i< num_elem; ++i)
    {
        getline(file, line);
        std::string num_vertice;
        std::istringstream ss(line);
        ss>>num_vertice; ss>>num_vertice; ss>>num_vertice;
        triangoli.push_back({0, 0, 0});
        for(unsigned k = 0; k<3; ++k){
            ss>>num_vertice;
            triangoli[i][k] = std::stoi(num_vertice)-1; 
        }
    }
    for(const auto & triangolo: triangoli)
    {
        
        for(unsigned k = 0; k<3; ++k)
        {
            for(unsigned j = 0; j<3 ; ++j)
            {
                dump_file<<vertici[triangolo[k]][j]<<" ";
            }
            dump_file<<std::endl;
        }
        dump_file<<std::endl;
    }
    if(type == "dg")
    {
        while(getline(file, line))
        {
            std::istringstream ss(line);
            std::string coordinata;
            ss>>coordinata;
            for (unsigned i = 0; i < 3; i++)
            {
                ss>>coordinata;
                dump_file<<coordinata<<" ";
            }
            dump_file<<std::endl;
            
        }
    }
    file.close();
    dump_file.close();

    return 0;
}
