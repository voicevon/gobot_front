#pragma once

#include "board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/eef/robot_eef_base.h"
#include "CNC/mover/stepper_servo.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"
#include "CNC/solution/solution_config_base.h"
#include "Robot/homer/single_axis_homer.h"

class CncBoardBase: public BoardBase{
    public:
        virtual void Init(bool is_on_reset);
        void SayHello() override;
        // virtual ActuatorStepper* GetActuator(char axis_name);
        virtual ActuatorBase* GetActuator(EnumAxis axis_name);
        virtual SingleAxisHomer* GetHomer(EnumAxis axis_name);
        virtual CncSolutionConfigBase* GetCncMechanic();
        virtual RobotEefBase* GetEef();
        virtual void EnableMotor(EnumAxis axis_name, bool enable_it);
        CncMoverBase* cnc_mover;   // TODO remove this memeber.??


    protected:

    private:

};
