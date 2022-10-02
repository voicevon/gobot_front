#pragma once

#include "mover_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"

class Mover_DualStepper: public MoverBase{
    public:
        void LinkStepper_asAlpha(ActuatorStepper* stepper);
        void LinkStepper_asBeta(ActuatorStepper* stepper);
        void LinkStepControl(StepControl* stepControl){this->__stepControl=stepControl;};
        
        bool ActuatorIsMoving(EnumAxis_Inverseinematic actuator) override;

        // void AllActuatorsMoveTo(MoveBlock* mov) override;
        // void AllActuatorsStop() override{};

        bool HasArrivedTargetPosition(){return false;};  //TODO: not false;


        
    protected:

    private:
        ActuatorStepper* __actuator_alpha;
        ActuatorStepper* __actuator_beta;
        StepControl* __stepControl;
};