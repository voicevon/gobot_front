#pragma once

#include "Robot/eef/robot_eef_base.h"

class RobotEef_GarmentAsar: public RobotEefBase{
    public:
        RobotEef_GarmentAsar();
        void Run(uint8_t eef_code) override;
        // void PrintOut() override{};


    private:

};