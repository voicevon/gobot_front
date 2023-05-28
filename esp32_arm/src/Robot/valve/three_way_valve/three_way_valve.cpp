#include "three_way_valve.h"

ThreeWayValve::ThreeWayValve(){
    __state = EnumState::STATE_IDLE;

}

void ThreeWayValve::LinkReciprocator(ReciprocatorBase* reciprocator){
    __reciprocator = reciprocator;
    __go_staight_stopper = reciprocator->GetTrigger_for_MaxPostion();
    __go_staight_stopper = reciprocator->GetTrigger_for_MinPosition();
}

void ThreeWayValve::SetMode_GoStraight(){
    __reciprocator->MoveToTriggerPosition(__go_staight_stopper);
    __state = EnumState::STATE_GO_STRAIGHT_SETTING;
}

void ThreeWayValve::SetMode_TurnRight(){
    __reciprocator->MoveToTriggerPosition(__turn_right_stopper);
    __state = EnumState::STATE_TURN_RIGHT_SETTING;
}


