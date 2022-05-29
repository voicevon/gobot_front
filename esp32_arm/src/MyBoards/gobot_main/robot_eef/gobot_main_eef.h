#pragma once

#include "CNC/robot_eef_base.h"



class RobotEef_GobotMain: public RobotEefBase{
    public:
        RobotEef_GobotMain();
        void Run(uint8_t eef_code) override;

    private:

};