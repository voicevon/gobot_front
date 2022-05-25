#pragma once

#include "obstacle_sensor_base.h"
#include <HCSR04.h> 

class ObstacleSensor_Hcsr04: public ObstacleSensorBase{
    public:
        ObstacleSensor_Hcsr04(const uint8_t HCSR04_PIN_TRIG, const uint8_t HCSR04_PIN_ECHO);

    protected:
        bool DetectObstacle() override;

    private:
        UltraSonicDistanceSensor* hcsr04;
};