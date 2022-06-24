#pragma once

#include "board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "RobotEef/robot_eef_base.h"
#include "CNC/cnc_mover/mover_stepper_servo.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"
#include "CNC/cnc_machine_base.h"
#include "cnc/single_axis_homer.h"

class CncBoardBase: public BoardBase{
    public:
        virtual void Init(bool is_on_reset);
        void SayHello() override;
        // virtual ActuatorStepper* GetActuator(char axis_name);
        virtual ActuatorBase* GetActuator(char axis_name);
        virtual SingleAxisHomer* GetHomer(char axis_name);
        virtual CncMachineBase* GetCncMechanic();
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
