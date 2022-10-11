#pragma once

#include "Robot/eef/robot_eef_base.h"
#include "../teeth_wh_board.h"

class TeethWarehouse_RobotEef: public RobotEefBase{
    public:
        TeethWarehouse_RobotEef(){};
        void Run(uint8_t eef_code) override;
        // void PrintOut() override;

    private:
        TeethWarehouse_Board* board;

};