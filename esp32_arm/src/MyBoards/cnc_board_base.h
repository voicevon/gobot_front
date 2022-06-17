#pragma once

#include "board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "cnc/single_axis_homer.h"
#include "RobotEef/robot_eef_base.h"
#include "CNC/cnc_mover/mover_stepper_servo.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"

class CncBoardBase: public BoardBase{
    public:
        virtual void Init(bool is_on_reset);
        void SayHello() override;
        virtual ActuatorStepper* GetJointStepper(char axis_name);
        virtual SingleAxisHomer* GetHomer(char axis_name);
        virtual RobotEefBase* GetEef();
        virtual void EnableMotor(char axis_name, bool enable_it);

        CncMoverBase* cnc_mover;


    protected:
        // void LinkCncMover(CncMoverBase* _cnc_mover){this->cnc_mover=_cnc_mover;};

        // use of deleted function 'StepControlBase<a, t>::StepControlBase(const StepControlBase<a, t>&) [with Accelerator = LinStepAccelerator; TimerField = TimerField]'
        // StepControl* GetStepControl(){return &this->_objStepControl;};
        // StepControl _objStepControl;

    private:

};
