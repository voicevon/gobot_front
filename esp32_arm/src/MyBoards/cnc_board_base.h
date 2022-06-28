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
        virtual ActuatorBase* GetActuator(EnumAxis axis_name);
        virtual SingleAxisHomer* GetHomer(EnumAxis axis_name);
        virtual CncSolutionConfigBase* GetCncMechanic();
        virtual RobotEefBase* GetEef();
        virtual void EnableMotor(EnumAxis axis_name, bool enable_it);
        CncMoverBase* cnc_mover;   // TODO remove this memeber.??


    protected:

    private:

};
