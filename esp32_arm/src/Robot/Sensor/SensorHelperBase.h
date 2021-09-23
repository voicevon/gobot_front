#pragma once
// #include "SimpleFOC/sensors/Encoder.h"
#include "SimpleFOC/common/base_classes/Sensor.h"

enum Formula {
    LinearEquation = 1,   // Out = Slope * ( Raw_value + offset)
};


class SensorHelperBase{
    public:
        void LinkSensor(Sensor* sensor){
            this->sensor = sensor;
        };
        /** 
         * Out = Slope * ( Raw_value + offset)
         */
        void InitFormula_LinearEquation(float slope,float offset){this->slope=slope; this->offset=offset;};
        virtual float GetMeanValue();
        virtual void SetCurrentMeanValueAs(float current_mean_value);
        
    protected:
        Formula formula;
        // for Y = slope * (X + offset).
        float slope = 1.0f;
        float offset = 0.0f;

        Sensor* sensor;
};