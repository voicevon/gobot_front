#include "HomeTriger.h"

HomeTriger::HomeTriger(uint8_t pinTriger, uint8_t trigeredState){
    this->pinTriger = pinTriger;
    this->trigeredState = trigeredState;
    pinMode(pinTriger, INPUT_PULLUP);
}

bool HomeTriger::IsTriged(){
    if (digitalRead(pinTriger) == trigeredState)
        return true;
    return false;
}
