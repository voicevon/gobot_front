#pragma once
#include "ESP32Step/src/TeensyStep.h"

struct motor_position{
    int alpha;
    int beta;
};


class RobotArm{
    public:

    protected:
        void MoveTo(int16_t x, int16_t y);
        virtual motor_position ik(int x, int y);

        Stepper* stepper_alpha;
        Stepper* stepper_beta;
        StepControl* steppers;
    private:

};