#ifndef BOARD_GOBOT_HOUSE_H_
#define BOARD_GOBOT_HOUSE_H_

#include "MyLibs/Frame/boardbase.hpp"
// #include "MyApps/Common/StepperDriver.h"
#include "ESP32Step/src/TeensyStep.h"
#define LEDS_COUNT_ON_BOARD 5

class Board_GobotHouse: public BoardBase<LEDS_COUNT_ON_BOARD>{
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