// #include "single_axis_homer.h"
#include "single_position_trigger.h"
#include "MyLibs/basic/logger.h"

SinglePositionTrigger::SinglePositionTrigger(uint8_t pinTriger, int trigeredState){
    this->pinTriger = pinTriger;
    this->trigeredState = trigeredState;
    pinMode(pinTriger, INPUT_PULLUP);
    this->__mcp23018 = nullptr;
}

SinglePositionTrigger::SinglePositionTrigger(Adafruit_MCP23X17* mcp_23018, uint8_t expanded_pinTriger, int trigeredState){
    this->__mcp23018 = mcp_23018;
    this->pinTriger = expanded_pinTriger;
    this->trigeredState = trigeredState;
    // Init expaned pin
    this->__mcp23018->pinMode(pinTriger, INPUT_PULLUP);
}

SinglePositionTrigger::SinglePositionTrigger(int trigeredState){
    this->trigeredState = trigeredState;
}

void SinglePositionTrigger::Init_mcp23018_gpio(Adafruit_MCP23X17* mcp_23018, uint8_t expanded_pinTriger){
    mcp_23018->pinMode(pinTriger, INPUT_PULLUP);
    this->pinTriger = expanded_pinTriger;
    this->__mcp23018 = mcp_23018;
}

bool SinglePositionTrigger::IsTriged(){
    // Read sensor pin
    int pin_value;
    if (this->__mcp23018 == nullptr)
        pin_value = digitalRead(this->pinTriger);
    else{
        pin_value = this->__mcp23018->digitalRead(this->pinTriger);
    }
    // Make a decision if is trigered.
    if (pin_value == trigeredState){
        this->trigered_counter++;
        if (this->trigered_counter > this->trigered_counter_max)
            return true;
    }
    else{
        this->trigered_counter = 0;
    }
    return false;
}

void SinglePositionTrigger::PrintOut(const char* title){
    Logger::Debug("SinglePositionTrigger::PrintOut()");
    Logger::Print("pin_triger", this->pinTriger);
}


