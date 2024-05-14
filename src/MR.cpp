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
            for(int i=0;i<num_blocks;i++){
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
            for(int i=0;i<num_nets;i++){
                int x,y,tx,ty;
                line.clear();
                
                std::getline(file,line);
                line.erase(0,line.find(' '));
                std::stringstream(line)>>x>>y>>tx>>ty;
                
                Net *temp=new Net(i,x,y,tx,ty);
                nets_.push_back(temp);
                
                //init the net order
                net_order.push_back(i);
            }
        }
        else line.clear();
    }
    path.resize(num_nets);
    DEBUG_MSG("Input file read successfully.");
}

int MR::routing(){
    // DEBUG_MSG("initializing map...");
    init_map();
    // DEBUG_MSG("map and blocks initialized.");

    DEBUG_MSG("before Running A* algorithm...");
    
    auto iter=net_order.begin();
    while(iter!=net_order.end()){
        if(!astar(*nets_[*iter])){
            DEBUG_MSG("no path found for net ");
            update_netorder(*iter);
            rip_up();
            init_map();
            iter=net_order.begin();
        }
        else iter=std::next(iter);
    }

    // int i=1;
    // for(auto node:path){
    //     std::cout<<"grid size: "<<node.size()-2<<'\n';
    //     std::cout<<"path for net"<<i<<": \n";
    //     for(auto point:node) std::cout<<point.first<<","<<point.second<<'\n';
    // }
    
    DEBUG_MSG("A* algorithm complete.");
    return 0;
}

bool MR::astar(Net &net){
    //the container to search
    std::vector<std::shared_ptr<Node>> Open;
    std::vector<std::shared_ptr<Node>> Closed;

    //unlock the pins of search net
    unlock_visited(net.get_source_x(),net.get_source_y());
    unlock_visited(net.get_target_x(),net.get_target_y());
    
    //init the start and goal node
    // DEBUG_MSG("init astar...");
    std::shared_ptr<Node> start=std::make_shared<Node>(net.get_source_x(),net.get_source_y());
    std::shared_ptr<Node> goal=std::make_shared<Node>(net.get_target_x(),net.get_target_y());
    
    //init start node g and h value,set parent to NULL
    start->set_g(0);
    start->set_h(heuristic(start->get_x(),start->get_y(),goal->get_x(),goal->get_y()));
    start->set_parent(NULL);
    
    //add into open list and heap
    Open.push_back(start);
    std::push_heap(Open.begin(),Open.end(),cmp());
    // DEBUG_MSG("init astar done...");
    
    //repeat until open list is empty(no path found)
    // DEBUG_MSG("start astar looping...");
    while(!Open.empty()){
        //Pick n_best node from Open list s.t. $f(n_best)<=f(n),for all in Open$
        std::shared_ptr<Node> tmp=Open.front();
        
        //Remove n_best from Open list and add into Closed list
        std::pop_heap(Open.begin(),Open.end(),cmp());
        Open.pop_back();
        Closed.push_back(tmp);
        
        //if n_best equal to goal node
        //backtrack to find the path and install it into the map
        //return true to indicate that path found
        if(tmp->get_x()==goal->get_x() && tmp->get_y() == goal->get_y()){
            DEBUG_MSG("found path...");
            while(tmp!=NULL){
                path[net.get_id()].push_back({tmp->get_x(),tmp->get_y()});
                set_visited(tmp->get_x(),tmp->get_y());
                tmp=tmp->get_parent();
            }
            return true;
        }

        //Expend n_best node:for all x\in neighbors(n_best) that not in Closed list
        //find neighbors from the map
        std::vector<std::shared_ptr<Node>> neighbors;
        find_neighbors(tmp->get_x(),tmp->get_y(),neighbors);
        
        //finding each neighbor and check if it is in closed list or not
        //if x not in Open list then add x to Open list
        //if x in Open list then check if $g(x)<g(n_best)+1$ then update $g(x)$ and $parent(x)$
        for(auto motion:neighbors){
            //if neighbor in closed list, continue
            if(in_list(Closed,motion)) continue;

            //discover a new node
            if(!in_list(Open,motion)){
                // DEBUG_MSG("find node not in open list...");
                motion->set_g(tmp->get_g()+1);
                motion->set_h(heuristic(motion->get_x(),motion->get_y(),goal->get_x(),goal->get_y()));
                motion->set_parent(tmp);
                Open.push_back(motion);
                std::push_heap(Open.begin(),Open.end(),cmp());
            }

            //if tmp have shorter distance than original parent of x, update g and parent of x
            else if(tmp->get_g()+1<motion->get_g()){
                DEBUG_MSG("Update G value of surrounding node in open list...");
                motion->set_g(tmp->get_g()+1);
                motion->set_parent(tmp);
                std::make_heap(Open.begin(),Open.end(),cmp());
            }
        }
    }
    //if no path found, return false
    return false;
}

auto MR::print_path(std::vector<std::pair<int,int>> &spath){
    std::vector<std::shared_ptr<Block>> Paths;
    int slow=spath.size()-1;
    DEBUG_MSG("Printing path...");
    for(int i=spath.size()-1;i>=0;i--){
        if(spath[slow].first!=spath[i].first&&spath[slow].second!=spath[i].second){
            Paths.push_back(std::make_shared<Block>(slow,spath[slow].first,spath[i+1].first,spath[slow].second,spath[i+1].second));
            slow=i+1;
            i++;
            // std::cout<<Paths[Paths.size()-1]->get_LeftDown_x()<<" "<<Paths[Paths.size()-1]->get_LeftDown_y()<<" "<<Paths[Paths.size()-1]->get_RightUp_x()<<" "<<Paths[Paths.size()-1]->get_RightUp_y()<<'\n';
        }
        else if(i==0){
            Paths.push_back(std::make_shared<Block>(slow,spath[slow].first,spath[i].first,spath[slow].second,spath[i].second));
            // std::cout<<Paths[Paths.size()-1]->get_LeftDown_x()<<" "<<Paths[Paths.size()-1]->get_LeftDown_y()<<" "<<Paths[Paths.size()-1]->get_RightUp_x()<<" "<<Paths[Paths.size()-1]->get_RightUp_y()<<'\n';
        }
    }
    DEBUG_MSG("Path printed success...");
    return Paths;
}

bool MR::in_list(std::vector<std::shared_ptr<Node>>& list,std::shared_ptr<Node>& node){
    for(auto &poi:list)
        if(poi->get_x()==node->get_x()&&poi->get_y()==node->get_y()) return true;
    return false;
}

void MR::init_map(){
    // DEBUG_MSG("Initializing map with row and col...");
    if(!map_.empty()){ DEBUG_MSG("Map clearing..."); map_.clear(); DEBUG_MSG("Map cleared..."); }
    map_=std::vector<std::vector<int>>(rows_,std::vector<int>(cols_,0));
    // DEBUG_MSG("Map initialize with row and col done...");
    //set up block and net, s.t. not line cross the object
    for(auto block : blocks_){
        set_block(block->get_LeftDown_x(),block->get_RightUp_x(),block->get_LeftDown_y(),block->get_RightUp_y());
    }
    for(auto net:nets_){
        set_visited(net->get_source_x(),net->get_source_y());
        set_visited(net->get_target_x(),net->get_target_y());
    }
    return;
}

void MR::set_block(int ldx,int rux,int ldy,int ruy){
    // DEBUG_MSG("Setting block...");
    for(int i=ldy;i<=ruy;i++){ for(int j=ldx;j<=rux;j++) map_[i][j]=-1; }
    // DEBUG_MSG("Block set done...");
}

void MR::find_neighbors(int x,int y,std::vector<std::shared_ptr<Node>>& neighbors){
    // DEBUG_MSG("Finding neighbors...");
    if(x+1<cols_&&!is_block(x+1,y)) neighbors.push_back(std::make_shared<Node>(x+1,y));
    if(x-1>=0&&!is_block(x-1,y)) neighbors.push_back(std::make_shared<Node>(x-1,y));
    if(y+1<rows_&&!is_block(x,y+1)) neighbors.push_back(std::make_shared<Node>(x,y+1));
    if(y-1>=0&&!is_block(x,y-1)) neighbors.push_back(std::make_shared<Node>(x,y-1));
    // DEBUG_MSG("Neighbors found...");
}

void MR::print_map(){
    for(int i=rows_-1;i>=0;i--){
        for(int j=0;j<cols_;j++) std::cout<<map_[i][j]<<" ";
        std::cout<<'\n'; 
    }
}

void MR::print_outfile(std::ofstream &outfile){
    if(!outfile.is_open()){ std::cout<<"Error: Output file not open.\n"; return; }
    for(auto net:nets_){
        outfile<<"net"<<net->get_id()<<" "<<path[net->get_id()].size()-2<<'\n';
        outfile<<"begin\n";
        auto lines=print_path(path[net->get_id()]);
        for(auto line:lines)
            outfile<<line->get_LeftDown_x()<<" "<<line->get_LeftDown_y()<<" "<<line->get_RightUp_x()<<" "<<line->get_RightUp_y()<<'\n';
        outfile<<"end\n";
    }
}

void MR::update_netorder(int prior){
    net_order.erase(std::find(net_order.begin(),net_order.end(),prior));
    net_order.push_front(prior);
    return;
}