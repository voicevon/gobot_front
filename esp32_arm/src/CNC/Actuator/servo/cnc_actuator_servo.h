#pragma once

#include "../cnc_actuator_base.h"
#include <ESP32Servo.h>


class CncActuatorServo: public CncActuatorBase{
    public:
        void LinkServo(Servo* servo);
        void SpinOnce();
        void UpdateMovement(MoveBlock_SingleActuator* movement) override;
        void ForceStop_G28_Only(bool update_target_position = true) override {};   //No point for servo actuator.
        void SetCurrentPositionAs(float new_position) override;
        float GetCurrentPosition() override;

    protected:

    private:
        Servo* __servo;
        float __target_position;
};
