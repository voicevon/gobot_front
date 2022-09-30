#pragma once

#include "mover_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"

class Mover_DualStepper: public MoverBase{
    public:
        // Mover_DualStepper(){};
        // void Init(CncBoardBase* board) override;
        void LinkStepper_asAlpha(ActuatorStepper* stepper);
        void LinkStepper_asBeta(ActuatorStepper* stepper);
        void LinkStepControl(StepControl* stepControl){this->__stepControl=stepControl;};
        // void LinkServo_asBeta(ActuatorServo* servo);
        // void PrintOut(const char* title) override;

        
        // void SetEefSpeed(float speed) override;
        // float GetSingleActuatorCurrentPosition_InCncUnit(char motor_name) override;
        // float GetAbsDistanceToTarget_InCncUnit() override;
        // void SetActuatorSpeed(char actuator_name, float rad_per_second) override;
        // void SetActuatorAcceleration(char axis, float accelleration) override;
        bool ActuatorIsMoving(char actuator) override;


        // void AllActuatorsMoveTo(uint8_t is_absolute_position, float* positions_in_cnc_unit) override;
        void AllActuatorsMoveTo(MoveBlock* move) override;
        // void SingleActuatorMoveTo(MoveBlock_SingleActuator* movement) override;
        void AllActuatorsStop() override;
        // void SingleActuatorStop(char motor_name) override;

        bool HasArrivedTargetPosition(){};

        // void SetActuatorCurrentCncPositionAs(char actuator_name, float as_current_position) override;
        // void Stop(char motor_name) override;
        // struct MotorFlag{

        // }
        
    protected:

    private:
        // Stepper* __stepper_alpha;
        ActuatorStepper* __actuator_alpha;
        ActuatorStepper* __actuator_beta;
        StepControl* __stepControl;
        // uint8_t __moving_motor_flags;
};