#pragma once

#include "Robot/eef/robot_eef_base.h"

class Vsc_RobotEef: public RobotEefBase{
    public:
        Vsc_RobotEef(){};
        void Run(uint8_t eef_code) override;
        void PrintOut() override;

    private:

};