#pragma once

#include "obstacle_sensor_base.h"
#include <Adafruit_VL53L0X.h>


class ObstacleSensor_VL53l0x: public ObstacleSensorBase{
    public:
        // ObstacleSensor_VL53l0x(TwoWire* i2c_bus, uint8_t i2c_address);
        ObstacleSensor_VL53l0x(){};
        void Init(Adafruit_VL53L0X* vl53l0x);

    protected:
        bool DetectObstacle() override;

    private:
        Adafruit_VL53L0X* __vl53lox;
};
