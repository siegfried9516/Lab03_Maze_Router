#include<iostream>
#include<fstream>

#include"MR.h"

int main(int argc, char *argv[]){

    std::ifstream Filein;
    Filein.open(argv[1]);
    if(!Filein.is_open()){ std::cout<<"Error: File not found"<<std::endl; return 1; }
    
    MR mr(Filein);

    mr.routing();
    
    // std::cout<<"rows and cols: "<<mr.get_rows()<<" "<<mr.get_cols()<<std::endl;
    // mr.print_map();

    return 0;
}