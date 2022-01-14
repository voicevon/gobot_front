#include "HomeHelper.h"

HomeHelper::HomeHelper(uint8_t pinTriger, int trigeredState){
    this->pinTriger = pinTriger;
    this->trigeredState = trigeredState;
    pinMode(pinTriger, INPUT_PULLUP);
}

bool HomeHelper::IsTriged(){
    if (digitalRead(this->pinTriger) == trigeredState){
    // Serial.print("[Debug] HomeHelper.IsTriged() = True\n");
        return true;
    }
    // Serial.print("[Debug] HomeHelper.IsTriged() = False\n");
    return false;
}

