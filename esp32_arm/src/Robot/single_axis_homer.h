#ifndef _HOME_TRIGER_H_
#define _HOME_TRIGER_H_

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>


class SingleAxisHomer{
    public:
        // For OC output sensor, trigeredState should be LOW
        SingleAxisHomer(uint8_t pinTriger, int trigeredState);
        SingleAxisHomer(Adafruit_MCP23X17* mcp_23018, uint8_t expanded_pinTriger, int trigeredState);
        SingleAxisHomer(int trigeredState);
        void Init_mcp23018(Adafruit_MCP23X17* mcp_23018, uint8_t expanded_pinTriger);
        // void SetHomePosition();   //???
        bool IsTriged();
        void SetTrigeredCountMax(int new_value){this->trigered_counter_max = new_value;};

    private:
        int trigered_counter = 0;
        int trigered_counter_max = 3;
        uint8_t pinTriger;
        int trigeredState;
        Adafruit_MCP23X17* __mcp23018;
};


#endif