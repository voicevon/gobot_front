#pragma once
#include <stdint.h>

 
class BranchNode{
    public:
        BranchNode(){
            this->NodeId = 0;
        }
        enum TASK{
            FOLLOW_LEFT,
            FOLLLOW_RIGHT,
            TURN_LEFT,
            TURN_RIGHT,
            LOADING,
            UNLOADING,
            SLEEPING,
            CHARGING,
        };
        BranchNode(uint16_t NodeId, TASK task ){
            this->NodeId = NodeId;
            this->task = task;
            // this->FollowLeft = follow_left;
            // this->ShouldPark = should_park;
            };
        uint16_t NodeId; 
        TASK task;
        bool GoingOnFollowLeft(){
            if (this->task == FOLLOW_LEFT || this->task == TURN_LEFT)
                return true;
            return false;
        };
        bool GoingOnFastMoving(){
            if (this->task == FOLLOW_LEFT || this->task == FOLLLOW_RIGHT)
                return true;
            return false;
        };
};


class TrackGraph{
    public:
        TrackGraph();
        bool AddNode(uint16_t NodeId, BranchNode::TASK task);
        bool RemoveNode(uint16_t NodeId);
        bool FetchNode(uint16_t NodeId, BranchNode* the_node );

    private:
        BranchNode __all_branch_nodes[20];
};