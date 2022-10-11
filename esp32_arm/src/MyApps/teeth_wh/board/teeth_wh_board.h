#pragma once
#include "CNC/board/cnc_board_base.h"
// #include "eef/teeth_wh_eef.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"


class TeethWarehouse_Board: public CncBoardBase{
    public:
        // For being a real PCB board.
        // TeethWarehouse_Board(){};
        void Init(bool is_on_reset) override;

        // For being an actuator and its components.

        // For being a CNC machine.
        // RobotEefBase* GetEef() override {return &this->__eef;};

        // We do nothing, just override the methods.
        void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) override {};
        void EnableVacuume(bool enable_it);

        void Test_PositionTriggers(int loops);

    protected:

    private:
        // TeethWarehouse_RobotEef __eef;
        
        PositionTrigger __all_position_triggers[4];

};