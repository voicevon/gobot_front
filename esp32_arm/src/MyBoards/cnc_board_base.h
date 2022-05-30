#pragma once

#include "board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "cnc/single_axis_homer.h"
#include "cnc/robot_eef_base.h"


class CncBoardBase: public BoardBase{
    public:
        virtual void Init(bool is_on_reset);
        void SayHello();
        virtual Stepper* GetStepper(char axis_name);
        virtual SingleAxisHomer* GetHomer(char axis_name);
        virtual RobotEefBase* GetEef();
        virtual void EnableMotor(char axis_name, bool enable_it);

    protected:
        // use of deleted function 'StepControlBase<a, t>::StepControlBase(const StepControlBase<a, t>&) [with Accelerator = LinStepAccelerator; TimerField = TimerField]'
        // StepControl* GetStepControl(){return &this->_objStepControl;};
        // StepControl _objStepControl;

    private:

};
