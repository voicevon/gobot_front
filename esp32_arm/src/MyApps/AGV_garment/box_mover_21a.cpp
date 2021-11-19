#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT


#include "box_mover_21a.h"

BoxMover_21a::BoxMover_21a(){

}

void BoxMover_21a::SpinOnce(){
    this->verticalMover->SpinOnce();
    this->angleMover->SpinOnce();
    switch (this->State){
        case RESETTING:
            if (verticalMover->AT_NORTH && angleMover->AT_SOURTH){
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