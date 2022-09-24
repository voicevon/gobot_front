#pragma once

#include <SimpleFOC.h>
#include "postitioon_sensor_base.h"

class PolorEncoder: public PositionSensorBase{
    public:
        PolorEncoder(){};
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
        float __raw_offset;
        float __slope_from_raw_to_cnc;

};