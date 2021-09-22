#pragma once
// #include "SimpleFOC/sensors/Encoder.h"
#include "SimpleFOC/common/base_classes/Sensor.h"

/**
 *    Position = slope * (sensor->getAngle + angle_offset);
 */
class SensorHelper{
    public:
        SensorHelper(float slope){
            this->slope = slope;
        }
        void LinkSensor(Sensor* sensor){
            this->sensor = sensor;
        };
        float GetAngle(){
            return sensor->getAngle() - this->angle_offset;
        };
        float GetPosition(){
            return slope * sensor->getAngle() + this->angle_offset;
        };

        void SetCurrentAngleAs(float position_offset){
            this->angle_offset = position_offset/this->slope - sensor->getAngle();
        };
    private:
        float slope = 1.0f;
        float angle_offset = 0.0f;
        Sensor* sensor;
};