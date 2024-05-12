#include"MR.h"

MR::MR(std::ifstream &file){
    int num_blocks,num_nets;
    std::string line;
    DEBUG_MSG("Reading input file...");
    while(std::getline(file,line)){
        if(line.find(".row")!=std::string::npos){
            line.erase(0,4);
            std::stringstream(line)>>rows_;
        }
        else if(line.find(".col")!=std::string::npos){
            line.erase(0,4);
            std::stringstream(line)>>cols_;
        }
        else if(line.find(".block")!=std::string::npos){
            line.erase(0,6);
            std::stringstream(line)>>num_blocks;
            for(int i=0;i<=num_blocks;i++){
                int Lx,Rx,Ly,Ry;
                line.clear();
                
                std::getline(file,line);
                std::stringstream(line)>>Lx>>Rx>>Ly>>Ry;
                
                Block* temp=new Block(i,Lx,Rx,Ly,Ry);
                blocks_.push_back(temp);
            }
        }
        else if(line.find(".net")!=std::string::npos){
            line.erase(0,4);
            std::stringstream(line)>>num_nets;
            for(int i=0;i<=num_nets;i++){
                int x,y,tx,ty;
                line.clear();
                
                std::getline(file,line);
                line.erase(0,line.find(' '));
                std::stringstream(line)>>x>>y>>tx>>ty;
                
                Net *temp=new Net(i,x,y,tx,ty);
                nets_.push_back(temp);

                net_order.push_back(i);
            }
        }
        else line.clear();
    }
    DEBUG_MSG("Input file read successfully.");
}

void MR::routing(){
    DEBUG_MSG("Running the initial mapping...");
    init_map();
    DEBUG_MSG("Initial mapping complete.");
}