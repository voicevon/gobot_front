#include "HomeTriger.h"

HomeTriger::HomeTriger(uint8_t pinTriger, uint8_t trigeredState){
    this->pinNumber = pinTriger;
    this->trigeredState = trigeredState;
    pinMode(pinTriger, INPUT_PULLUP);
}

bool HomeTriger::IsTriged(){
    if (digitalRead(this->pinNumber) == trigeredState)
        return true;
    return false;
}
