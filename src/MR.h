#ifndef MR_H
#define MR_H

#define DEBUG

#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>

#include"block.h"
#include"net.h"
#include"map.h"

//#define Debug
#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while (false)
#else
#define DEBUG_MSG(str) do { } while (false)
#endif

class Block;
class Net;
class Map;

class MR{
    public:
        //constructor & destructor
        MR(std::ifstream &file);
        ~MR(){
            for(auto block : blocks_) delete block;
            for(auto net : nets_) delete net;
            blocks_.clear();
            nets_.clear();
            net_order.clear();
        }
        //getters
        int get_rows() const { return rows_; }
        int get_cols() const { return cols_; }
        //setters
        void set_rows(int rows) { rows_ = rows; }
        void set_cols(int cols) { cols_ = cols; }
        //wtf
        void routing();
        void init_map(){
            map_.init_map(rows_,cols_);
            for(auto block : blocks_)
                map_.set_block(block->get_LeftDown_x(),block->get_RightUp_x(),block->get_LeftDown_y(),block->get_RightUp_y());
        }
        void print_map(){ map_.print_map(); }
    private:
        int rows_,cols_;
        std::vector<Block*> blocks_;
        std::vector<Net*> nets_;
        std::vector<int> net_order;
        Map map_;
};

#endif