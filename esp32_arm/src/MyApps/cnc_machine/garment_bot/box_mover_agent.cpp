#include "box_mover_agent.h"


//  All message to the top level boxmover is gcode. the top-level boxmover is a mqtt bridge essentially.
//  Is this idea good or bad?  How to analysis it? 
void GarmentBoxMoverAgent::ToState(BoxMoverState state){
    switch (state)
    {
    case GarmentBoxMoverAgent::BoxMoverState::LOADING:
        // publish a mqtt message:  load
        break;
    
    default:
        break;
    }
}
