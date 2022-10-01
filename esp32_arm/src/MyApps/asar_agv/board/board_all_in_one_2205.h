#pragma once
#include "MyBoards/board_base.h"
#include "sub_board_agv/board_agv_2205.h"
#include "sub_board_cnc2205/board_cnc_2205.h"
#include "sub_board_asrs/board_asrs_2205.h"


class BoardAllInOne: public BoardBase{
    public:
        BoardAllInOne(){};
        Board2205Cnc cnc_board;
        Board2205Agv agv;
        BoardAsrs asrs;
        void Init(bool is_on_reset) override;
        void Test_ScanI2cBuses(int loop_count);
        void Test_Blink();
        Adafruit_MCP23X17* Get_Mcp23018(){return &this->__mcp23018;};

        // void TurnLedOn(bool turn_on);


    private:
        TwoWire __i2c_bus_main = TwoWire(0);
        TwoWire __i2c_bus_ext = TwoWire(1);
        Adafruit_MCP23X17 __mcp23018;
        bool blink_flag;

};
