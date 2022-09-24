// #include "single_axis_homer.h"
#include "single_position_trigger.h"
#include "MyLibs/basic/logger.h"

SinglePositionTrigger::SinglePositionTrigger(uint8_t pin_trigger, int triggered_state){
    this->__pin_trigger = pin_trigger;
    this->__triggered_state = triggered_state;
    pinMode(pin_trigger, INPUT_PULLUP);
    this->__mcp23018 = nullptr;
}

SinglePositionTrigger::SinglePositionTrigger(Adafruit_MCP23X17* mcp_23018, uint8_t expanded_pin_trigger, int triggered_state){
    this->__mcp23018 = mcp_23018;
    this->__pin_trigger = expanded_pin_trigger;
    this->__triggered_state = triggered_state;
    // Init expaned pin
    this->__mcp23018->pinMode(expanded_pin_trigger, INPUT_PULLUP);
}

SinglePositionTrigger::SinglePositionTrigger(int triggeredState){
    this->__triggered_state = triggeredState;
}

void SinglePositionTrigger::Init_mcp23018_gpio(Adafruit_MCP23X17* mcp_23018, uint8_t expanded_pin_trigger){
    mcp_23018->pinMode(expanded_pin_trigger, INPUT_PULLUP);
    this->__pin_trigger = expanded_pin_trigger;
    this->__mcp23018 = mcp_23018;
}

bool SinglePositionTrigger::IsTriggered(){
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

void SinglePositionTrigger::PrintOut(const char* title){
    Logger::Debug("SinglePositionTrigger::PrintOut()");
    Logger::Print("pin_triger", this->__pin_trigger);
}


