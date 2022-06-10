#pragma once

#include "RobotEef/robot_eef_base.h"
#include "../board_pins_ver3.11.h"


class RobotEef_GobotHouse: public RobotEefBase{
    public:
        RobotEef_GobotHouse();
        void Run(uint8_t eef_code) override;

    private:

};