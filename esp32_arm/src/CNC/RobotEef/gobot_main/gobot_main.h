#pragma once

#include "../eef_base.h"


class RobotEef_GobotMain: public RobotEefBase{
    public:
        RobotEef_GobotMain();
        void Run(uint8_t eef_code) override;

    private:

};