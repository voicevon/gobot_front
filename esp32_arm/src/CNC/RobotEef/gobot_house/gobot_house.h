#pragma once

#include "../eef_base.h"


class RobotEef_GobotHouse: public RobotEefBase{
    public:
        RobotEef_GobotHouse();
        void Run(uint8_t eef_code) override;

    private:

};