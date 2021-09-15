#ifndef BOARD_GOBOT_CHESSBOARD_H_
#define BOARD_GOBOT_CHESSBOARD_H_

#include "boardbase.h"
// #include "MyApps/Common/StepperDriver.h"
#include "ESP32Step/src/TeensyStep.h"


class Board_GobotChessboard: public BoardBase{
    public:
        Board_GobotChessboard();

        // StepperDriver* stepper_alhpa;
        // StepperDriver* stepper_beta;
        Stepper* stepper_alhpa;
        Stepper* stepper_beta;
        void Test_home_sensor();

    private:
};

#endif