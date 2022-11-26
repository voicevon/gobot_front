#pragma once

#include <SimpleFOC.h>
#include "postitioon_sensor_base.h"

class RotaryEncoder: public PositionSensorBase{
    public:
        RotaryEncoder(){};
        void LinkRawSensor(Encoder* simple_foc_encoder);
        Encoder* GetRawSensor(){return this->__simple_foc_encoder;};
        void SetCurrentPosition(float current_cnc_position) override;
        float GetCurrentPosition() override;
        float GetCurrentVelocity();
        void SetupFormular(float slope, float raw_offset);

    protected:
        // float _GetRawValue() override;

    private:
        Encoder* __simple_foc_encoder;
        float __formular_raw_offset;
        float __formular_slope_from_raw_to_cnc;
        float __ModTwoPi(float origin_value);

};