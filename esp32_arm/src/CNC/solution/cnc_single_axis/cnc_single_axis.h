#pragma once

#include "MyBoards/cnc_board_base.h"
#include "CNC/solution/cnc_base.h"
#include "cnc_single_axis_machine.h"


//This is a virtual class. No Constructor.
class CncSingleAxis: public CncBase{
    public:
        void Init(CncBoardBase* board) override;
        void RunG28(EnumAxis axis) override;
        void RunG1(Gcode* gcode) override;

        bool GetCurrentPosition(FkPositionBase* position_fk) override{return false;};  
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;

    protected:
        CncSingleAxis_Machine* _mechanic;  //TODO:  uinify name:  cncConfig??

    private:
        char __AXIS_NAME = 'A';
        virtual void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override;

        void RunG6(Gcode* gcode) override{} ; 
        void _running_G1() override;
        void _running_G28() override;
        void RunM123(uint8_t eef_channel, uint8_t eef_action) override;
        void RunM84() override;
        std::string GetHomeTrigerStateString() override {return " ";};

        FkPosition_A __current_fk_position;

};