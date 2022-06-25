#pragma once

#include "MyBoards/agv_board_base.h"
#include "../board_pins/board_pins_ver_2_0.h"
#include "AGV/sensor_moving_track/track_sensor_dual_9960.h"
#include "AGV/sensor_obstacle/obstacle_sensor_vl53l0x.h"
#include "AGV/sensor_map_site/smart_rfid_reader.h"
#include "AGV/mover_driver/mover_dual_wheel.h"
#include "AGV/mover_driver/single_wheel_driver/single_wheel_h_bridge_pwm_driver.h"
#include "AGV/track_light/light_ws2812b.h"
#include <MFRC522.h>

class Board2205Agv: public AgvBoardbase{
    public:
        Board2205Agv(){};
        void Init(bool is_on_reset);
        void Init(TwoWire* i2c_bus_main, TwoWire* i2c_bus_ext);
        TrackSensor_Dual9960* Get_Dual9960(){return &this->__dual_9960;};
        ObstacleSensor_VL53l0x* Get_Obstacle_Vl53l0x(){return &this->__obstacle_vl53l0x;};
        MoverDualWheel* Get_DualWheelDriver(){return &this->__dual_wheel;};
        Light_WS2812B* Get_Ws2812B(){return &this->__track_light;};

        ObstacleSensorBase* GetObstacleSensor() override{return &this->__obstacle_vl53l0x;};
        TrackSensorBase* GetTrackSensor() override {return &this->__dual_9960;};
        MapSiteReaderBase* GetMapSiteReader() override {return &this->__rfid_reader;};
        MoverBase* GetMover() override {return &this->__dual_wheel;};
        LightBase* GetTrackLight() override {return &this->__track_light;};
        // SmartRfidReader* GetRfidReader() override{return &this->__rfid_reader;};
        // float Get_Battery_volt();
        float ReadBatteryVolt() override;


    private:
        Adafruit_APDS9960 __left_aps9960;
        Adafruit_APDS9960 __right_aps9960;
        TrackSensor_Dual9960 __dual_9960;

        Adafruit_VL53L0X __vl53l0x;
        ObstacleSensor_VL53l0x __obstacle_vl53l0x;

	    SingleWheel_HBridgePwmDriver __left_wheel = SingleWheel_HBridgePwmDriver(PIN_WHEEL_PWM_LEFT_2205, PIN_WHEEL_DIR_LEFT_2205);
	    SingleWheel_HBridgePwmDriver __right_wheel = SingleWheel_HBridgePwmDriver(PIN_WHEEL_PWM_RIGHT_2205, PIN_WHEEL_DIR_RIGHT_2205);
        MoverDualWheel __dual_wheel;

        Adafruit_NeoPixel __neo_pixel =Adafruit_NeoPixel(16, PIN_WS2812B, NEO_GRB + NEO_KHZ800); 
        Light_WS2812B __track_light = Light_WS2812B(16);


        MFRC522 __mfrc522 = MFRC522(PIN_RFID_RESET);
        SmartRfidReader __rfid_reader;
};
