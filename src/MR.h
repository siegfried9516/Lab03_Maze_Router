#ifndef MR_H
#define MR_H

#include<fstream>
#include<sstream>
#include<queue>
#include<algorithm>
#include<cmath>
#include<vector>
#include<list>
#include<iostream>
#include<memory>

#include"block.h"
#include"net.h"
#include"node.h"

//#define Debug
#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while (false)
#else
#define DEBUG_MSG(str) do { } while (false)
#endif

class Block;
class Net;
class Map;
class Node;

class MR{
    public:
        //constructor & destructor
        MR(std::ifstream &file);
        ~MR(){
            for(auto block : blocks_) delete block;
            for(auto net : nets_) delete net;
            map_.clear();
            blocks_.clear();
            nets_.clear();
            net_order.clear();
            path.clear();
        }
        //getters
        int get_rows() const { return rows_; }
        int get_cols() const { return cols_; }
        //setters
        void set_rows(int rows) { rows_ = rows; }
        void set_cols(int cols) { cols_ = cols; }
        //wtf
        int routing();
        int heuristic(int sx,int sy,int tx,int ty){ return abs(tx-sx)+abs(ty-sy); }
        bool astar(Net &net);
        auto print_path(std::vector<std::pair<int,int>> &spath);
        bool in_list(std::vector<std::shared_ptr<Node>>& list,std::shared_ptr<Node>& node);
        void print_outfile(std::ofstream &outfile);
        void update_netorder(int prior);
        void rip_up(){ path.clear(); path.resize(nets_.size()); }
        //map functions
        //Set & Get
        void set_block(int ldx,int rux,int ldy,int ruy);
        void init_map();
        void unlock_visited(int x,int y){ map_[y][x]=0; }
        void set_visited(int x,int y){ map_[y][x]=1; }
        void print_map();
        bool is_block(int x,int y){ return (map_[y][x]!=0)?true:false; }
        void find_neighbors(int x,int y,std::vector<std::shared_ptr<Node>>& neighbors);
    private:
        int rows_,cols_;
        std::vector<std::vector<int>> map_;
        std::vector<Block*> blocks_;
        std::vector<Net*> nets_;
        std::list<int> net_order;
        std::vector<std::vector<std::pair<int,int>>> path;
};

#endif