#pragma once
#include <stdint.h>

 
class RoadBranchNode{
    public:

        enum TASK{
            // FOLLOW_LEFT,
            // FOLLLOW_RIGHT,
            // TURN_LEFT,
            // TURN_RIGHT,

            
            SHORT_CUT_ONLY,  // No load, unload, sleep, charge.  
            LOAD,
            UNLOAD,
            SLEEP,
            CHARGE,
        };
        RoadBranchNode();
        RoadBranchNode(uint16_t NodeId, TASK task);
        uint16_t NodeId; 
        TASK task;
        // bool GoingOnFollowLeft();
        // bool GoingOnFastMoving();
};


class RoadGraph{
    public:
        RoadGraph();
        bool AddNode(uint16_t NodeId, RoadBranchNode::TASK task);
        bool RemoveNode(uint16_t NodeId);
        bool FetchNode(uint16_t NodeId, RoadBranchNode* the_node );

    private:
        RoadBranchNode __all_branch_nodes[20];
};