#pragma once
#include "CNC/reciprocator/reciprocator_base.h"

class DcMotorIR_Reciprocator:public ReciprocatorBase{
    public:
        DcMotorIR_Reciprocator();

    protected:
        void _MoveToTriggerPosition() override;
        // void _MoveToEncoderPosition(uint32_t encoder_position) override;
    private:

};
