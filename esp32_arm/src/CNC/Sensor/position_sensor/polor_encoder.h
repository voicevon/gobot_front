#pragma once

#include <SimpleFOC.h>
#include "../sensor_base.h"

class PolorEncoder: public SensorBase{
    public:
        PolorEncoder(){};
        void LinkRawSensor(Encoder* simple_foc_encoder);
        Encoder* GetRawSensor(){return this->__simple_foc_encoder;};
        void SetCurrentPosition(float current_cnc_position) override;
        float GetCurrentPosition() override;
        void SetupFormular(float slope, float raw_offset);

    protected:
        // float _GetRawValue() override;

    private:
        Encoder* __simple_foc_encoder;
        float __raw_offset;
        float __slope;

};