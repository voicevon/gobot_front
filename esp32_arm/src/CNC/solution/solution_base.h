#pragma once

#include "Robot/eef/eef_standard_code.h"
#include "CNC/gcode/gcode_consumer.h"
#include "MyBoards/cnc_board_base.h"
#include "solution_config_base.h"
#include "Robot/homer/single_axis_homer.h"
#include "../coordinate/coordinate_base.h"
#include "../coordinate/cnc_axis.h"


enum class CncState{
    IDLE,
    RUNNING_G1,
    RUNNING_G4,
    RUNNING_G28
};

class CncSolutionBase: public GcodeConsumer{
    public:
        CncState State = CncState::IDLE;
        void RunGcode(Gcode* gcode);
        void SpinOnce();
        virtual void Init(CncBoardBase* board);
        void LinkEef(RobotEefBase* eef){this->__eef=eef;};
        // void LinkStepControl(StepControl* stepControl){this->_stepControl=stepControl;};
        void RunG28(EnumAxis axis);
        virtual bool GetCurrentPosition(FkPositionBase* position_fk);
        virtual float GetDistanceToTarget_FK();
        virtual float GetDistanceToTarget_IK();
        void SayHello();

    protected:
        void SpinOnce_BaseExit();
        virtual void IK(FkPositionBase* from_fk, IkPositionBase* to_ik);
        virtual void FK(IkPositionBase* from_ik,FkPositionBase* to_fk);
        virtual void RunG1(Gcode* gcode);   //None blocking, move backgroundly.
        virtual void RunG6(Gcode* gcode);   //Block mode
        void RunG4(Gcode* gcode);
        virtual std::string GetHomeTrigerStateString();
        void RunM42(uint8_t pin_number, uint8_t pin_value);
        void RunM84();
        // virtual void RunM123(uint8_t eef_channel, EefAction eef_action);
        void RunM123(uint8_t eef_channel, uint8_t eef_action);
        bool is_absolute_position = true;

        void _running_G1();
        void _running_G28();
        virtual EnumAxis ConvertToEnum(char axis);
        virtual void _SetCurrentPositionAsHome(EnumAxis homing_axis){};  //TODO:  remove defination
		virtual void _RunG28_CombinedFk(EnumAxis axis){};

        EnumAxis _homing_axis_name;
        
        CncSolutionConfigBase* _config_base;    //???
        CncBoardBase* _board;
        SingleAxisHomer* __current_homer;


    private:
        int test_int;
        RobotEefBase* __eef;
        void _base_spin_once();
        void __running_G4();
        void __HomeSingleAxis(EnumAxis axis);
        long __g4_start_timestamp;
        int __g4_time_second;

        /* Just for fun, don't remove below comment.
        void * __output_message2;
        void (* __output_message3); 
        void OnFinishedGcode2(void(*callback)()) {__output_message2 = callback;};
        void OnFinishedGcode3(void(*callback)()) {__output_message2 = callback;};
        void OnFinishedGcode4(void(*callback())) {__output_message3 = callback;};
        void OnFinishedGcode5(void(*callback)()) {__output_message3 = callback;};
        */

};

