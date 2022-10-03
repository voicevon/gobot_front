#pragma once

#include "mover_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "CNC/Actuator/servo/actuator_servo.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"

class Mover_StepperServo: public MoverBase{
    public:
        void LinkStepControl(StepControl* stepControl){this->__stepControl=stepControl;};
        void LinkStepper_asAlpha(ActuatorStepper* stepper);
        void LinkServo_asBeta(ActuatorServo* servo);
        // bool ActuatorIsMoving(EnumAxis_Inverseinematic actuator) override;
        // bool HasArrivedTargetPosition() override{return false;};


        // void AllActuatorsMoveTo(MoveBlock* move) override;
        // void AllActuatorsStop() override;
        
    protected:

    private:
        ActuatorStepper* __actuator_alpha;
        ActuatorServo* __actuator_beta;
        StepControl* __stepControl;
};