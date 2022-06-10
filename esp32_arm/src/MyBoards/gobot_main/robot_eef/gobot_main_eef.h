#pragma once

#include "RobotEef/robot_eef_base.h"
#include "../board_ver1.2.h"

class RobotEef_GobotMain: public RobotEefBase{
    public:
        RobotEef_GobotMain();
        void Run(uint8_t eef_code) override;

    private:

};