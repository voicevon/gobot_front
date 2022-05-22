#pragma once

#include "obstacle_sensor_base.h"
#include "Wire.h"


class ObstacleSensor_Gp2y0e03: public ObstacleSensorBase{
    public:
        ObstacleSensor_Gp2y0e03(TwoWire* i2c_bus, uint8_t i2c_address);

    protected:
        bool DetectObstacle() override;

    private:
};
