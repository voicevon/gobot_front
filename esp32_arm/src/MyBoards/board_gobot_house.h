#ifndef BOARD_GOBOT_HOUSE_H_
#define BOARD_GOBOT_HOUSE_H_

#include "boardbase.h"
#include "MyApps/Common/StepperDriver.h"

class Board_GobotHouse: public BoardBase{
    public:
        StepperDriver* alhpa;
        StepperDriver* beta;
        Board_GobotHouse();
        void Test_home_sensor();

    private:
};

#endif