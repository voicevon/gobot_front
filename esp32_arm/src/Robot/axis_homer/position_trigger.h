#ifndef _HOME_TRIGER_H_
#define _HOME_TRIGER_H_

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>


// TODO:  rename to PositionTrigger
class PositionTrigger{
    public:
        // For OC output sensor, trigeredState should be LOW
        PositionTrigger(uint8_t pinTriger, int triggeredState);
        PositionTrigger(Adafruit_MCP23X17* mcp_23018, uint8_t expanded_pin_trigger, int triggered_state);
        PositionTrigger(int trigeredState);
        void Init_mcp23018_gpio(Adafruit_MCP23X17* mcp_23018, uint8_t expanded_pinTriger);
        bool IsTriggered();   //TODO:  rename to IsFired()
        void SetTriggeredCountMax(int new_value){this->__triggered_counter_max = new_value;};
        void PrintOut(const char* title);
        void SetTriggerPosition(float trigger_position){this->__trigger_position_in_cnc_unit=trigger_position;};
        float GetTriggerPosition(){return this->__trigger_position_in_cnc_unit;};

    private:
        int __triggered_counter = 0;
        int __triggered_counter_max = 3;
        uint8_t __pin_trigger;
        int __triggered_state;
        Adafruit_MCP23X17* __mcp23018;
        float __trigger_position_in_cnc_unit = 0;
};


#endif