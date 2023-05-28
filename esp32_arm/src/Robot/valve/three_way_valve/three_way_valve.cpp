#include "three_way_valve.h"


void ThreeWayValve::SpinOnce_Statemachine(){
    __reciprocator->SpinOnce_Statemahcine();
    if (__state == EnumState::STATE_GO_STRAIGHT_SETTING){
        if (__reciprocator->GetState() == ReciprocatorBase::EnumState::READY){
            // TODO:  publish a mqtt message here? 
            __state = EnumState::STATE_GO_STRAIGHT_READY;
        }
    }
    if(__state == EnumState::STATE_GO_STRAIGHT_READY){
        __led_go_straight_idle_green->TurnOn();
        __state = EnumState::STATE_GO_STRAIGHT_IDLE;
    }
    if (__state == EnumState::STATE_GO_STRAIGHT_IDLE){
        // if (__inlet)
    }
}


ThreeWayValve::ThreeWayValve(){
    __state = EnumState::STATE_IDLE;
    __turn_off_all_leds();
}

void ThreeWayValve::__turn_off_all_leds(){
    __led_go_straight_idle_green->TurnOff();
    __led_go_straight_inlet_red->TurnOff();
    __led_go_straight_outlet_blue->TurnOff();

    __led_turn_right_idle_green->TurnOff();
    __led_turn_right_inlet_red->TurnOff();
    __led_turn_right_outlet_blue->TurnOff();
}


void ThreeWayValve::LinkReciprocator(ReciprocatorBase* reciprocator){
    __reciprocator = reciprocator;
    __go_staight_stopper = reciprocator->GetTrigger_for_MaxPostion();
    __go_staight_stopper = reciprocator->GetTrigger_for_MinPosition();
}

void ThreeWayValve::LinkLeds(BinaryOutput_GPIO* led_go_straight_idle_green, BinaryOutput_GPIO* led_go_straight_inlet_red, BinaryOutput_GPIO* led_go_straight_outlet_blue,
                    BinaryOutput_GPIO* led_turn_right_idle_green, BinaryOutput_GPIO* led_turn_right_inlet_red, BinaryOutput_GPIO* led_turn_right_outlet_blue ){

    __led_go_straight_idle_green = led_go_straight_idle_green;
    __led_go_straight_inlet_red = led_go_straight_inlet_red;
    __led_go_straight_outlet_blue = led_go_straight_outlet_blue;

    __led_turn_right_idle_green = led_turn_right_idle_green;
    __led_turn_right_inlet_red = led_turn_right_inlet_red;
    __led_turn_right_outlet_blue = led_turn_right_outlet_blue;
}

void ThreeWayValve::SetMode_GoStraight(){
    __reciprocator->MoveToTriggerPosition(__go_staight_stopper);
    __state = EnumState::STATE_GO_STRAIGHT_SETTING;
}

void ThreeWayValve::SetMode_TurnRight(){
    __reciprocator->MoveToTriggerPosition(__turn_right_stopper);
    __state = EnumState::STATE_TURN_RIGHT_SETTING;
}


