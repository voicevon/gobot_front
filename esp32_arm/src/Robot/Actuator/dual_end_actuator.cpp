#include "dual_end_actuator.h"


void DualEndActuator::SpinOnce(){
    int pin_level;
    switch (this->State){
        case START:
        case AT_NORTH:
        case AT_SOUTH:
            break;
        case MOVING_TO_NORTH:
            pin_level = digitalRead(this->__pin_endstop_north);
            if (pin_level && this->__north_triger_on_low){
                this->State = AT_NORTH;
                driver->Stop();
            }
            break;
            
        case MOVING_TO_SOUTH:
            pin_level = digitalRead(this->__pin_endstop_south);
            if (pin_level && this->__south_triger_on_low){
                this->State = AT_SOUTH;
                driver->Stop();
            }
            break;

    }
}

void DualEndActuator::MoveToNorth(){
    this->driver->MoveAtSpeed(this->TargetSpeed, true);
    this->State = MOVING_TO_NORTH;
}

void DualEndActuator::MoveToSouth(){
    this->driver->MoveAtSpeed(this->TargetSpeed, false);
    this->State = MOVING_TO_SOUTH;
}

void DualEndActuator::Init_NorthEndstop(uint8_t pin, bool triger_on_low){
    pinMode(pin,INPUT_PULLUP);
    this->__pin_endstop_north = pin;
    this->__north_triger_on_low = triger_on_low;
}

void DualEndActuator::Init_SouthEndstop(uint8_t pin, bool triger_on_low){
    pinMode(pin,INPUT_PULLUP);
    this->__pin_endstop_south = pin;
    this->__south_triger_on_low = triger_on_low;
}
