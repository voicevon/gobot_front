#pragma once
#include "MyBoards/board_base.h"
#include "board_agv_2205.h"
#include "board_cnc_2205.h"
#include "board_asrs_2205.h"


class BoardAllInOne: public BoardBase{
    public:
        BoardAllInOne(){};
        Board2205Cnc cnc;
        Board2205Agv agv;
        BoardAsrs asrs;
        void Init(bool is_on_reset) override;
        void ScanI2cBuses();
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
