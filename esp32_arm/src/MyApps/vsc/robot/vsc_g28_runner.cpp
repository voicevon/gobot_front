#include "vsc_g28_runner.h"

void Vsc_G28_Runner::SetMoveBlock_ToHome(EnumAxis_Inverseinematic axis, MoveBlock* mb){
    Logger::Debug("Vsc_G28_Runner::SetMoveBlock_ToHome() ---- Inverse axis");
    Logger::Print("axis", axis);
    MoveBlock_SingleActuator* aa;
    switch (axis){
        case AXIS_ALPHA:
            Logger::Print("Vsc_G28_Runner::SetMoveBlock_ToHome()  point", 21);
            mb->DeepReset_ToDefault();
            Logger::Print("Vsc_G28_Runner::SetMoveBlock_ToHome()  point", 22);
            aa = &mb->MoveBlocks[0];
            Logger::Print("Vsc_G28_Runner::SetMoveBlock_ToHome()  point", 23);
            aa->IsAbsTargetPosition = false;
            aa->TargetPosition = 200;
            aa->Speed = 555;
            aa->Acceleration = 33;
            break;
        default:
            Logger::Error(" Vsc_G28_Runner::SetMoveBlock_ToHome() Unknown axis");
            
            break;
    }
    Logger::Print("Vsc_G28_Runner::SetMoveBlock_ToHome()  point", 99);
}

void Vsc_G28_Runner::SetMoveBlock_ToHome(EnumAxis_ForwardKinematic axis, MoveBlock* mb){
    Logger::Debug("Vsc_G28_Runner::SetMoveBlock_ToHome()   ---- Forward axis" );
    Serial.print(char(axis));
    Logger::Print("\taxis", char(axis));
    MoveBlock_SingleActuator* aa;
    switch (axis){
        case AXIS_X:
            Logger::Print("Vsc_G28_Runner::SetMoveBlock_ToHome()  point", 21);
            mb->DeepReset_ToDefault();
            Logger::Print("Vsc_G28_Runner::SetMoveBlock_ToHome()  point", 22);
            aa = &mb->MoveBlocks[0];
            Logger::Print("Vsc_G28_Runner::SetMoveBlock_ToHome()  point", 23);
            aa->IsAbsTargetPosition = false;
            aa->TargetPosition = 200;
            aa->Speed = 555;
            aa->Acceleration = 33;
            break;
        default:
            Logger::Error(" Vsc_G28_Runner::SetMoveBlock_ToHome() Unknown axis");
            
            break;
    }
    Logger::Print("Vsc_G28_Runner::SetMoveBlock_ToHome()  point", 99);
}
