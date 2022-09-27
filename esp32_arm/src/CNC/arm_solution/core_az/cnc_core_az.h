#pragma once

#include "CNC/arm_solution/arm_solution_base.h"
#include "ESP32Step/src/TeensyStep.h"
// #include "Robot/homer/single_axis_homer.h"
// #include "CNC/Sensor/position_trigger/position_triggers.h"
#include "CNC/arm_solution/axis_homer/axis_homer.h"
#include "config_base.h"
#include "MyBoards/board_base.h"



#define VERTICAL_ENDSTOP 15
#define ANGLE_ENDSTOP 2

#define PIN_STEP_ALPHA 5
#define PIN_DIR_ALPHA 19
#define PIN_STEP_BETA 4
#define PIN_DIR_BETA 17



class CncSolution_CoreAZ: public ArmSolutionBase{
    public:

        // void Init(CncBoardBase* board) override;
         void RunG1(Gcode* gcode) override;

        bool GetCurrentPosition(FkPositionBase* position_fk) override{return false;};  
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;

    protected:

        //Override private
        virtual void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override;
        virtual void _SetCurrentPositionAsHome(EnumAxis homing_axis) override;
        void RunG28_CombinedAxis(EnumAxis axis);
        void RunG6(Gcode* gcode) override{} ; 
        std::string GetHomeTrigerStateString() override {return " ";};

        FkPosition_ZW __current_fk_position;

        PositionTrigger* __homing_helper;
        PositionTrigger* objHomeHelper_vertical; // = SingleAxisHomer(VERTICAL_ENDSTOP, LOW);
        PositionTrigger* objHomeHelper_angle; // = SingleAxisHomer(ANGLE_ENDSTOP, LOW);        
        CncSolution_CoreAZConfigBase* _config;


};