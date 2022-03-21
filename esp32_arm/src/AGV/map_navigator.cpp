# include "map_navigator.h"

TrackGraph::TrackGraph(){
    for(int i=0; i<20; i++){
        this->__all_branch_nodes[i].NodeId = 0;
    }
}

bool TrackGraph::AddNode(uint16_t NodeId, BranchNode::TASK task){
    for(int i=0; i<20; i++){
        if (this->__all_branch_nodes[i].NodeId == 0){
            this->__all_branch_nodes[i].NodeId = NodeId;
            this->__all_branch_nodes[i].task = task;
            // this->__all_branch_nodes[i].FollowLeft = follow_left;
            // this->__all_branch_nodes[i].ShouldPark = should_park;
            return true;
        }
    }
    return false;   
}

bool TrackGraph::RemoveNode(uint16_t NodeId){
    for(int i=0; i<20; i++){
        if (this->__all_branch_nodes[i].NodeId == NodeId){
            this->__all_branch_nodes[i].NodeId = 0;
            return true;
        }
    }
    return false;
}


bool TrackGraph::FetchNode(uint16_t NodeId, BranchNode* the_node ){
    return false;
}