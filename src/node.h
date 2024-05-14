#ifndef NODE_H
#define NODE_H

#include<memory>

class Node{
    public:
        Node(int coor_x,int coor_y):x_(coor_x),y_(coor_y),g_(0),h_(0),parent_(nullptr){}
        //getter
        int get_x() const{return x_;}
        int get_y() const{return y_;}
        int get_g() const{return g_;}
        int get_h() const{return h_;}
        auto get_parent() const{return parent_;}
        //setter
        void set_x(int coor_x){x_ = coor_x;}
        void set_y(int coor_y){y_ = coor_y;}
        void set_g(int g){g_=g;}
        void set_h(int h){h_ = h;}
        void set_parent(std::shared_ptr<Node> parent){parent_ = parent;}
    private:
        int g_;   //the distance from the start node (real)
        int h_;   //the shortest manhattan distance to the goal node (ideal)
        int x_;   //coordinates in map
        int y_;   //coordinates in map
        std::shared_ptr<Node> parent_;   //pointer to the parent node
};

struct cmp{
    bool operator()(const std::shared_ptr<Node> a, const std::shared_ptr<Node> b){ return a->get_g() + a->get_h() > b->get_g() + b->get_h(); }
};

#endif