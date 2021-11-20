#include "dual_end_actuator.h"


DualEndActuator::DualEndActuator(){
    State = this->START;
}

void DualEndActuator::SpinOnce(){
    int pin_level;
    switch (this->State){
        case START:
        case AT_NORTH:
        case AT_SOUTH:
            break;
        case MOVING_TO_NORTH:
            pin_level = digitalRead(this->__pin_endstop_north);
            if (pin_level ^ this->__north_normal_is_high){
                this->State = AT_NORTH;
                driver->Stop();
            }
            break;
            
        case MOVING_TO_SOUTH:
            pin_level = digitalRead(this->__pin_endstop_south);
            if (pin_level ^ this->__south_normal_is_high){
                this->State = AT_SOUTH;
                driver->Stop();
            }
            break;

    }
}

void DualEndActuator::MoveToNorth(){
    if (this->State != AT_NORTH){
        this->driver->MoveAtSpeed(this->TargetSpeed, true);
        this->State = MOVING_TO_NORTH;
    }
}

void DualEndActuator::MoveToSouth(){
    Serial.print("   fffffffff  ");
    if (this->State != AT_SOUTH){
    Serial.print( this->TargetSpeed );
        this->driver->MoveAtSpeed(this->TargetSpeed, false);
        this->State = MOVING_TO_SOUTH;
    }
}

void DualEndActuator::Init_NorthEndstop(uint8_t pin, bool normal_is_high){
    pinMode(pin,INPUT_PULLUP);
    this->__pin_endstop_north = pin;
    this->__north_normal_is_high = normal_is_high;
}

void DualEndActuator::Init_SouthEndstop(uint8_t pin, bool normal_is_high){
    pinMode(pin,INPUT_PULLUP);
    this->__pin_endstop_south = pin;
    this->__south_normal_is_high = normal_is_high;
}

void DualEndActuator::Report(){
    bool n = digitalRead(this->__pin_endstop_north);
    bool s = digitalRead(this->__pin_endstop_south);
    Serial.print("\n  State = ");
    Serial.print(this->State);
    Serial.print("  Endstops (n,s)   (");
    Serial.print(n);
    Serial.print(",");
    Serial.print(s);
    Serial.println(")");
}
