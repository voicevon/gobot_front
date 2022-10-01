// #include "single_axis_homer.h"
#include "position_trigger.h"
#include "MyLibs/basic/logger.h"

PositionTrigger::PositionTrigger(uint8_t pin_trigger, int triggered_state){
    this->__pin_trigger = pin_trigger;
    this->__triggered_state = triggered_state;
    pinMode(pin_trigger, INPUT_PULLUP);
    this->__mcp23018 = nullptr;
}

PositionTrigger::PositionTrigger(Adafruit_MCP23X17* mcp_23018, uint8_t expanded_pin_trigger, int triggered_state){
    this->__mcp23018 = mcp_23018;
    this->__pin_trigger = expanded_pin_trigger;
    this->__triggered_state = triggered_state;
    // Init expaned pin
    this->__mcp23018->pinMode(expanded_pin_trigger, INPUT_PULLUP);
}

PositionTrigger::PositionTrigger(int triggeredState){
    this->__triggered_state = triggeredState;
}

void PositionTrigger::Init_mcp23018_gpio(Adafruit_MCP23X17* mcp_23018, uint8_t expanded_pin_trigger){
    mcp_23018->pinMode(expanded_pin_trigger, INPUT_PULLUP);
    this->__pin_trigger = expanded_pin_trigger;
    this->__mcp23018 = mcp_23018;
}

bool PositionTrigger::IsTriggered(){
    // Logger::Debug("PositionTrigger::IsTriggered() is entering...");
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


