#include "spring_maker_g28_runner.h"

void Spring_G28_Runner::SetMoveBlock_ToHome(EnumAxis axis, MoveBlock* mb){
    MoveBlock_SingleActuator* aa;
    switch (axis){
        case AXIS_ALPHA:
            mb->DeepReset_ToDefault();
            aa = &mb->MoveBlocks[0];
            aa->IsAbsTargetPosition = false;
            aa->TargetPosition = 200;
            aa->Speed = 555;
            aa->Acceleration = 33;
            break;
        default:
            Logger::Error(" SpringMaker_G28_Runner::SetMoveBlock_ToHome() Unknown axis");
            
            break;
    }
}
