#ifndef _HOME_TRIGER_H_
#define _HOME_TRIGER_H_

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>


class SinglePositionTriger{
    public:
        // For OC output sensor, trigeredState should be LOW
        SinglePositionTriger(uint8_t pinTriger, int trigeredState);
        SinglePositionTriger(Adafruit_MCP23X17* mcp_23018, uint8_t expanded_pinTriger, int trigeredState);
        SinglePositionTriger(int trigeredState);
        void Init_mcp23018_gpio(Adafruit_MCP23X17* mcp_23018, uint8_t expanded_pinTriger);
        bool IsTriged();   //TODO:  rename to IsFired()
        void SetTrigeredCountMax(int new_value){this->trigered_counter_max = new_value;};
        void PrintOut(const char* title);

    private:
        int trigered_counter = 0;
        int trigered_counter_max = 3;
        uint8_t pinTriger;
        int trigeredState;
        Adafruit_MCP23X17* __mcp23018;
};


#endif