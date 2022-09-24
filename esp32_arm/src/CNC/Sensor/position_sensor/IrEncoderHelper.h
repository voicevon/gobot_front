#pragma once


#include "SensorHelperBase.h"
#include "IREncoder.h"

/**
 *    Position = slope * (sensor->getAngle + offset);
 */


class IrEncoderHelper: public SensorHelperBase{
    public:
        IrEncoderHelper(){};

        float GetMeanValue() override{
            return slope * sensor->getAngle() + this->offset;
        };

        void SetCurrentMeanValueAs(float position_offset) override{
            this->offset = position_offset/this->slope - sensor->getAngle();
        };
        
    private:

};