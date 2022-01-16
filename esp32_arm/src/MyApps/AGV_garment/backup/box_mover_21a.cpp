#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT


#include "box_mover_21a.h"




void BoxMover_21a::Init_VerticalMover(DualEndActuator* actuator, uint8_t pin_endstop_north, uint8_t pin_endstop_south, DriverBase* driver){
    this->verticalMover = actuator;
    actuator->TargetSpeed = 250;
    actuator->Init_NorthEndstop(pin_endstop_north,false);
    actuator->Init_SouthEndstop(pin_endstop_south, false);
    actuator->LinkDriver(driver);
}

void BoxMover_21a::Init_AngleMover(DualEndActuator* actuator, uint8_t pin_endstop_north, uint8_t pin_endstop_south, DriverBase* driver){
    this->angleMover = actuator;
    actuator->TargetSpeed = 220;
    actuator->Init_NorthEndstop(pin_endstop_north,false);
    actuator->Init_SouthEndstop(pin_endstop_south, false);
    actuator->LinkDriver(driver);
}

void BoxMover_21a::SpinOnce(){
    this->verticalMover->SpinOnce();
    this->angleMover->SpinOnce();
    switch (this->State){
        case RESETTING:
            if (verticalMover->AT_NORTH && angleMover->AT_SOUTH){
                this->State = READY_TO_LOAD;
            }
            break;
        case LOADING:
            if(false){
                this->State = LOADED;
            }
            break;
        case LOADED:
            if (true){
                this->verticalMover->MoveToNorth();
                this->State = VERTICAL_UP;
            }
            break;
        case VERTICAL_UP:
            if (this->verticalMover->State == DualEndActuator::AT_NORTH){
                this->State = READY_TO_UNLOAD;
            }
            break;
        case UNLOADING:
            if(false){
                this->State = UNLOADED;
            }
            break;
        default:
            break;
    }
}

void BoxMover_21a::ResetToLoad(){
    this->verticalMover->MoveToSouth();
    this->angleMover->MoveToSouth();
}

void BoxMover_21a::LoadBox(){
    this->verticalMover->MoveToNorth();
}
        
#endif