#include "three_way_valve.h"

ThreeWayValve::ThreeWayValve(){
    __reciprocator.LinkActuator(&__dc_motor);

}

void ThreeWayValve::SetMode_GoStraight(){
    
    __state = EnumState::STATE_GO_STRAIGHT_SETTING;
}

void ThreeWayValve::SetMode_TurnRight(){

}