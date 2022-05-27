#pragma once

#include "MyLibs/board_base.h"
#include "AGV/sensor_moving_track/track_sensor_dual_9960.h"
#include "AGV/sensor_obstacle/obstacle_sensor_vl53l0x.h"
#include "board_pins_ver_2_0.h"

class BoardPart_Agv: public BoardbaseAgv{
    public:
        BoardPart_Agv(){};
        void Init(TwoWire* i2c_bus_main, TwoWire* i2c_bus_ext);
        TrackSensor_Dual9960* Get_Dual9960(){return &this->__dual_9960;};
        ObstacleSensor_VL53l0x* Get_Obstacle_Vl53l0x(){return &this->__obstacle_vl53l0x;};

    private:
        // Adafruit_MCP23X17* __mcp23018;
        // TwoWire* __i2c_bus;
        Adafruit_VL53L0X __vl53l0x;
        Adafruit_APDS9960 __left_aps9960;
        Adafruit_APDS9960 __right_aps9960;

        TrackSensor_Dual9960 __dual_9960;
        ObstacleSensor_VL53l0x __obstacle_vl53l0x;

};
