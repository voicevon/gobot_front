#include "box_mover_agent.h"

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
