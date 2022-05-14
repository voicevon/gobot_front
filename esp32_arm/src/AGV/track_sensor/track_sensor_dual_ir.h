#pragma once
#include "track_sensor_base.h"


class TrackSensor_DualIR: public TrackSensorBase{
    public:
        TrackSensor_DualIR(uint8_t pin_positive_adc, uint8_t pin_negative_adc);
        int16_t ReadAlignmentError() override;


    private:
        uint8_t __pin_positive_adc;
        uint8_t __pin_negative_adc;


};