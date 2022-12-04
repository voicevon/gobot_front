#pragma once

#include "../cnc_actuator_base.h"
#include <ESP32Servo.h>


class CncActuatorServo: public CncActuatorBase{
    public:
        // void LinkServo(Servo* servo, bool is_inversed_dir);
        void LinkServo(Servo* servo);

        void SpinOnce();
        void UpdateMovement(MoveBlock_SingleActuator* movement) override;

        void ForceStop() override {};   //No point for servo actuator.



    protected:

    private:
        Servo* __servo;
};
