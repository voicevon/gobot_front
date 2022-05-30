#pragma once

// #include "MyLibs/board_base.h"
#include "MyBoards/board_base.h"
#include "AGV/sensor_moving_track/track_sensor_dual_9960.h"
#include "AGV/sensor_obstacle/obstacle_sensor_vl53l0x.h"
#include "AGV/mover_driver/mover_dual_wheel.h"
#include "AGV/mover_driver/single_wheel_driver/single_wheel_h_bridge_pwm_driver.h"
#include "board_pins_ver_2_0.h"

class Board2205Agv: public BoardbaseAgv{
    public:
        Board2205Agv(){};
        void Init(bool is_on_reset);
        void Init(TwoWire* i2c_bus_main, TwoWire* i2c_bus_ext);
        TrackSensor_Dual9960* Get_Dual9960(){return &this->__dual_9960;};
        ObstacleSensor_VL53l0x* Get_Obstacle_Vl53l0x(){return &this->__obstacle_vl53l0x;};
        MoverDualWheel* Get_DualWheelDriver(){return &this->__dual_wheel;};
        
        ObstacleSensorBase* GetObstacleSensor() override{return &this->__obstacle_vl53l0x;};
        TrackSensorBase* GetTrackSensor() override {return &this->__dual_9960;};
        MoverBase* GetMover() override {return &this->__dual_wheel;};
        
    private:
        // Adafruit_MCP23X17* __mcp23018;
        // TwoWire* __i2c_bus;
        Adafruit_VL53L0X __vl53l0x;
        Adafruit_APDS9960 __left_aps9960;
        Adafruit_APDS9960 __right_aps9960;

        TrackSensor_Dual9960 __dual_9960;
        ObstacleSensor_VL53l0x __obstacle_vl53l0x;
	    SingleWheel_HBridgePwmDriver __left_wheel = SingleWheel_HBridgePwmDriver(PIN_WHEEL_PWM_LEFT_2205, PIN_WHEEL_DIR_LEFT_2205);
	    SingleWheel_HBridgePwmDriver __right_wheel = SingleWheel_HBridgePwmDriver(PIN_WHEEL_PWM_RIGHT_2205, PIN_WHEEL_DIR_RIGHT_2205);
        MoverDualWheel __dual_wheel;
};
