#include "position_trigger.h"
#include "von/utility/logger.h"

void PositionTrigger::Init(char name, uint8_t pin_trigger, int triggered_state){
    this->MyName = name;
    this->__pin_trigger = pin_trigger;
    this->__triggered_state = triggered_state;
    pinMode(pin_trigger, INPUT_PULLUP);
    this->__mcp23018 = nullptr;
}

void PositionTrigger::Init(char name, Adafruit_MCP23X17* mcp_23018, uint8_t expanded_pin_trigger, int triggered_state){
    this->MyName = name;
    this->__mcp23018 = mcp_23018;
    this->__pin_trigger = expanded_pin_trigger;
    this->__triggered_state = triggered_state;
    // Init expaned pin
    this->__mcp23018->pinMode(expanded_pin_trigger, INPUT_PULLUP);
}

bool PositionTrigger::IsFired(){
    // Logger::Debug("PositionTrigger::IsFired() is entering...");
    // Logger::Print("__pin_trigger", __pin_trigger);
    // Read sensor pin
    int pin_value;
    if (this->__mcp23018 == nullptr)
        pin_value = digitalRead(this->__pin_trigger);
    else{
        pin_value = this->__mcp23018->digitalRead(this->__pin_trigger);
    }
    // Make a decision if is trigered.
    if (pin_value == this->__triggered_state){
        this->__triggered_counter++;
        if (this->__triggered_counter > this->__triggered_counter_max)
            return true;
    }
    else{
        this->__triggered_counter = 0;
    }
    return false;
}

void PositionTrigger::PrintOut(const char* title){
    Logger::Debug("PositionTrigger::PrintOut()");
    Logger::Print("pin_triger", this->__pin_trigger);
}


