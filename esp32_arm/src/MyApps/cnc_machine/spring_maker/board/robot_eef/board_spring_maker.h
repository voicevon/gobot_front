#pragma once

#include "Robot/eef/robot_eef_base.h"


class RobotEef_SpringMaker: public RobotEefBase{
    public:
        RobotEef_SpringMaker();
        void Run(uint8_t eef_code) override;
    private:

};