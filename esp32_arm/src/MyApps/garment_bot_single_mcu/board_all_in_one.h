#pragma once

#include <MyLibs/board_base.h>
#include "board_agv.h"
#include "board_cnc.h"


class BoardAllInOne: public BoardBase{
    public:
        BoardAllInOne(){};
        void Init();
        BoardPart_Cnc cnc = BoardPart_Cnc();
        BoardPart_Agv agv = BoardPart_Agv();
        void BlinkTest();
        Adafruit_MCP23X17* Get_Mcp23018(){return &this->__mcp23018;};

        void TurnLedOn(bool turn_on);
        float Get_Battery_volt();


    private:
        TwoWire __i2c_bus_main = TwoWire(0);
        TwoWire __i2c_bus_ext = TwoWire(1);
        Adafruit_MCP23X17 __mcp23018;
        bool blink_flag;

};
