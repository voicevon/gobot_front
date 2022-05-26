#pragma once
#include "all_devices.h"
#ifdef USING_BOARD_AGV_SINGLE_BOARD_VER_2_0


#include <MyLibs/board_base.h>
#include "board_agv.h"
#include "board_cnc.h"


class BoardSingleMcu_ver2_0: public BoardBase{
    public:
        BoardSingleMcu_ver2_0(){};
        void Init();
        BoardPart_Cnc cnc = BoardPart_Cnc();
        BoardPart_Agv agv = BoardPart_Agv();
        void BlinkTest();


        Adafruit_MCP23X17* Get_Mcp23018(){return &this->__mcp23018;};
        Adafruit_VL53L0X* Get_Vl53l0x(){return &this->__vl53l0x;};
        Adafruit_APDS9960* Get_Apds9960_left(){return &this->__left_aps9960;};
        Adafruit_APDS9960* Get_Apds9960_right(){return &this->__right_aps9960;};
        void TurnLedOn(bool turn_on);
        float Get_Battery_volt();


    private:
        TwoWire __i2c_bus_main = TwoWire(0);
        TwoWire __i2c_bus_ext = TwoWire(1);
        Adafruit_MCP23X17 __mcp23018;
        Adafruit_VL53L0X __vl53l0x;
        Adafruit_APDS9960 __left_aps9960;
        Adafruit_APDS9960 __right_aps9960;

        bool blink_flag;

};

#endif