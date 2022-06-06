#pragma once
#include "MyBoards/board_base.h"
#include "board_agv_2205.h"
#include "board_cnc_2205.h"
#include "board_asrs_2205.h"


class BoardAllInOne: public BoardBase{
    public:
        BoardAllInOne(){};
        void Init(bool is_on_reset) override;
        Board2205Cnc cnc = Board2205Cnc();
        Board2205Agv agv = Board2205Agv();
        BoardAsrs asrs = BoardAsrs();
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
