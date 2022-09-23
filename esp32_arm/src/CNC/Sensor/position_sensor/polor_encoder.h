#pragma once

#include <SimpleFOC.h>
#include "../sensor_base.h"

class PolorEncoder: public SensorBase{
    public:
        PolorEncoder(){};
        void LinkRawSensor(Encoder* simple_foc_encoder);
        Encoder* GetRawSensor(){return this->__simple_foc_encoder;};


    protected:
        float _GetRawValue() override;

    private:
        Encoder* __simple_foc_encoder;

};