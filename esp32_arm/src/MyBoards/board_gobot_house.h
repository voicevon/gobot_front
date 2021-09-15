#ifndef BOARD_GOBOT_HOUSE_H_
#define BOARD_GOBOT_HOUSE_H_

#include "boardbase.h"
// #include "MyApps/Common/StepperDriver.h"
#include "ESP32Step/src/TeensyStep.h"


class Board_GobotHouse: public BoardBase{
    public:
        // StepperDriver* stepper_alhpa;
        // StepperDriver* stepper_beta;
        Stepper* stepper_alhpa;
        Stepper* stepper_beta;
        Board_GobotHouse();
        void Test_home_sensor();

    private:
};

#endif