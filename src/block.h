#ifndef BLOCK_H
#define BLOCK_H

class Block{
    public:
        Block(int id,int LD_x,int RU_x,int LD_y,int RU_y){
            id_ = id;
            LeftDown_x = LD_x;
            RightUp_x = RU_x;
            LeftDown_y = LD_y;
            RightUp_y = RU_y;
        }
        ~Block(){}
        //Set & Get functions
        void set_id(int id){id_=id;}
        void set_LeftDown_x(int x){LeftDown_x=x;}
        void set_LeftDown_y(int y){LeftDown_y=y;}
        void set_RightUp_x(int x){RightUp_x=x;}
        void set_RightUp_y(int y){RightUp_y=y;}
        int get_id(){return id_;}
        int get_LeftDown_x(){return LeftDown_x;}
        int get_LeftDown_y(){return LeftDown_y;}
        int get_RightUp_x(){return RightUp_x;}
        int get_RightUp_y(){return RightUp_y;}
    private:
        int id_;
        int LeftDown_x,LeftDown_y,RightUp_x,RightUp_y;
};

#endif