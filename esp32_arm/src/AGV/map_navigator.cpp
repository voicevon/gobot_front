# include "map_navigator.h"

RoadBranchNode::RoadBranchNode(){
    this->NodeId = 0;
}
RoadBranchNode::RoadBranchNode(uint16_t NodeId, TASK task ){
    this->NodeId = NodeId;
    this->task = task;
}


RoadGraph::RoadGraph(){
    for(int i=0; i<20; i++){
        this->__all_branch_nodes[i].NodeId = 0;
    }
}

bool RoadGraph::AddNode(uint16_t NodeId, RoadBranchNode::TASK task){
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

bool RoadGraph::RemoveNode(uint16_t NodeId){
    for(int i=0; i<20; i++){
        if (this->__all_branch_nodes[i].NodeId == NodeId){
            this->__all_branch_nodes[i].NodeId = 0;
            return true;
        }
    }
    return false;
}


bool RoadGraph::FetchNode(uint16_t NodeId, RoadBranchNode* the_node ){
    return false;
}