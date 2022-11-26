#pragma once

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>
#include "MyLibs/array/array_base.h"

class PositionTrigger: public ArrayItem{
    public:
        // For OC output sensor, Fired State should be LOW
        PositionTrigger(){};
        // When Connect to Esp32 GPIO. 
        void Init(char name, uint8_t pinTriger, int triggeredState);
        //When connect to MCP23X17/18
        void Init(char name, Adafruit_MCP23X17* mcp_23018, uint8_t expanded_pin_trigger, int triggered_state);
        bool IsFired();  
        void SetTriggeredCountMax(int new_value){this->__triggered_counter_max = new_value;};
        void PrintOut(const char* title);
        char __short_name;   //??

    private:
        int __triggered_counter = 0;
        int __triggered_counter_max = 3;
        uint8_t __pin_trigger;
        int __triggered_state;
        Adafruit_MCP23X17* __mcp23018;
};

