#pragma once

#include "accel_stepper/AccelStepper.h"
#include "accel_stepper/MultiStepper.h"
#include "actions.h"



// Up to 10 steppers can be handled as a group by MultiStepper

class Arm{
    public:
        Arm(){};
        void Home(unsigned char axis);
        void Init(void);
        void SpinOnce(ArmAction action);
        void pick_place_park(ArmAction arm_action);

    private:
        // EG X-Y position bed driven by 2 steppers
        // Alas its not possible to build an array of these with different pins for each :-(
        AccelStepper stepper1;
        AccelStepper stepper2;
        MultiStepper steppers;

    protected:

};
