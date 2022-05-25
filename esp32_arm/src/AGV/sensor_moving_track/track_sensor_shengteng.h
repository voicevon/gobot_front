#pragma once

#include "track_sensor_base.h"

// https://item.taobao.com/item.htm?spm=a1z09.2.0.0.73742e8doTPlqO&id=658370820044&_u=l2023p00mp0504
class TrackSensor_Shengteng: public TrackSensorBase{
    public:
        TrackSensor_Shengteng(){};
        int ReadError_FromRight(uint8_t* rxBuffer);
        int16_t ReadAlignmentError() override {return 0;};

    private:
        // uint8_t slave_address = 0x3f;
        // uint8_t RxBuffer[1];

};