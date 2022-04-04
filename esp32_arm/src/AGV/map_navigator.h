#pragma once
#include <stdint.h>

 
class BranchNode{
    public:

        enum TASK{
            // FOLLOW_LEFT,
            // FOLLLOW_RIGHT,
            // TURN_LEFT,
            // TURN_RIGHT,
            SHORT_CUT,
            LOADING,
            UNLOADING,
            SLEEPING,
            CHARGING,
        };
        BranchNode();
        BranchNode(uint16_t NodeId, TASK task);
        uint16_t NodeId; 
        TASK task;
        // bool GoingOnFollowLeft();
        // bool GoingOnFastMoving();
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