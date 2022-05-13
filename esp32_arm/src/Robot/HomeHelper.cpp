#include "HomeHelper.h"

HomeHelper::HomeHelper(uint8_t pinTriger, int trigeredState){
    this->pinTriger = pinTriger;
    this->trigeredState = trigeredState;
    pinMode(pinTriger, INPUT_PULLUP);
    this->__mcp23018 = nullptr;
}

HomeHelper::HomeHelper(Adafruit_MCP23X17* mcp_23018, uint8_t expanded_pinTriger, int trigeredState){
    this->__mcp23018 = mcp_23018;
    this->pinTriger = expanded_pinTriger;
    this->trigeredState = trigeredState;
    // Init expaned pin
    this->__mcp23018->pinMode(pinTriger,INPUT_PULLUP);
}


bool HomeHelper::IsTriged(){
    // Read sensor pin
    int pin_value;
    if (this->__mcp23018 == nullptr)
        pin_value = digitalRead(this->pinTriger);
    else
        pin_value = this->__mcp23018->digitalRead(this->pinTriger);

    // Make a decision if is trigered.
    if (pin_value == trigeredState){
    // Serial.print("[Debug] HomeHelper.IsTriged() = True\n");
        this->trigered_counter++;
        if (this->trigered_counter > this->trigered_counter_max)
            return true;
    }
    else{
        this->trigered_counter = 0;
    }
    // Serial.print("[Debug] HomeHelper.IsTriged() = False\n");
    return false;
}

