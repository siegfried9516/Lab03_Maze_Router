#ifndef NET_H
#define NET_H

class Net{
    public:
        Net(int id, int sou_x, int sou_y, int tar_x, int tar_y){
            id_=id;
            source_x=sou_x;
            source_y=sou_y;
            target_x=tar_x;    
            target_y=tar_y;
        }
        ~Net(){}
        //set & get functions
        void set_id(int id){id_=id;}
        void set_source_x(int sou_x){source_x=sou_x;}
        void set_source_y(int sou_y){source_y=sou_y;}
        void set_target_x(int tar_x){target_x=tar_x;}
        void set_target_y(int tar_y){target_y=tar_y;}
        int get_source_x(){return source_x;}
        int get_source_y(){return source_y;}
        int get_target_x(){return target_x;}
        int get_target_y(){return target_y;}
        int get_id(){return id_;}
    private:
        int id_;
        int source_x,source_y,target_x,target_y;
};

#endif