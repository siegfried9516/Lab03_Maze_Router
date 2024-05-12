#ifndef MAP_H
#define MAP_H

#include<vector>
#include<iostream>

class Map{
    public:
        Map(){};
        ~Map(){ for(auto row:map_) row.clear(); }
        //Set & Get
        int get_rows(){return rows_;}
        int get_cols(){return cols_;}
        void init_map(int rows,int cols){ rows_=rows; cols_=cols; map_=std::vector<std::vector<int>>(rows,std::vector<int>(cols,0)); }
        void set_block(int ldx,int rux,int ldy,int ruy){ for(int i=ldy;i<=ruy;i++){ for(int j=ldx;j<=rux;j++) map_[i][j]=-1; } }
        //func functions
        void print_map(){ for(int i=rows_-1;i>=0;i--){ for(int j=0;j<cols_;j++) std::cout<<map_[i][j]<<" "; std::cout<<std::endl; } }
    private:
        int rows_,cols_;
        std::vector<std::vector<int>> map_;
};

#endif