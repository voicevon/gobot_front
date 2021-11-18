#pragma once

#include "track_sensor_base.h"


class TrackSensor_I2C: public TrackSensorBase{
    public:
        TrackSensor_I2C();
        int ReadError_ToRight() override;

    private:
        uint8_t slave_address = 3;
        uint8_t RxBuffer[1];

};