#pragma once

// #include "CNC/robot_eef_base.h"
#include "RobotEef/robot_eef_base.h"



class RobotEef_GobotHouse: public RobotEefBase{
    public:
        RobotEef_GobotHouse();
        void Run(uint8_t eef_code) override;

    private:

};