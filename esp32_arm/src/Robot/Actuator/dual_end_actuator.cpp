#include "dual_end_actuator.h"


void DualEndActuator::SpinOnce(){

}

void DualEndActuator::MoveToNorth(){
    this->driver->Start(100, true);
}

void DualEndActuator::MoveToSouth(){
    this->driver->Start(100, false);

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
