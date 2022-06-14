#pragma once

#include "board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "cnc/single_axis_homer.h"
#include "RobotEef/robot_eef_base.h"
#include "CNC/cnc_mover/cnc_mover_base.h"

class CncBoardBase: public BoardBase{
    public:
        virtual void Init(bool is_on_reset);
        void SayHello() override;
        // virtual Stepper* GetStepper(char axis_name);
        virtual SingleAxisHomer* GetHomer(char axis_name);
        virtual RobotEefBase* GetEef();
        virtual void EnableMotor(char axis_name, bool enable_it);
        // virtual void AllMotorsMoveTo(bool is_absolute_position, float* motor_position, int motors_count);
        // virtual void AllMotorStop();
        // virtual void SingleMotorMoveTo(char motor_name, float motor_position);
        // virtual float GetMotorPosition(char motor_name);
        // virtual void SetMotorPosition(char motor_name, float as_current_position);
        // virtual float GetDistanceToTarget();
        CncMoverBase* _cnc_mover;

    protected:
        // use of deleted function 'StepControlBase<a, t>::StepControlBase(const StepControlBase<a, t>&) [with Accelerator = LinStepAccelerator; TimerField = TimerField]'
        // StepControl* GetStepControl(){return &this->_objStepControl;};
        // StepControl _objStepControl;

    private:

};
