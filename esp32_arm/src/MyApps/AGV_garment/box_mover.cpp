
#include "box_mover.h"

void BoxMover::SpinOnce(){
    // this->verticalMover->SpinOnce();
    // this->angleMover->SpinOnce();
    switch (this->State){
        case RESETTING:
            // if (verticalMover->AT_NORTH && angleMover->AT_SOUTH){
            //     this->State = READY_TO_LOAD;
            // }
            break;
        case LOADING:
            if(false){
                this->State = LOADED;
            }
            break;
        case LOADED:
            if (true){
                // this->verticalMover->MoveToNorth();
                // this->State = VERTICAL_UP;
            }
            break;
        case VERTICAL_UP:
            // if (this->verticalMover->State == DualEndActuator::AT_NORTH){
            //     this->State = READY_TO_UNLOAD;
            // }
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


void BoxMover::ResetToLoad(){

}
void BoxMover::LoadBox(){

}
void BoxMover::UnloadBox(){

}

