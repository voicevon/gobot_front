#pragma once

// #include "RobotEef/robot_eef_base.h"
#include "Robot/eef/robot_eef_base.h"



class RobotEef_SmokeMachine: public RobotEefBase{
    public:
        RobotEef_SmokeMachine();
        void Run(uint8_t eef_code) override;

    private:

};