#pragma once

#include "../actuator_base.h"
#include <ESP32Servo.h>


class ActuatorServo: public ActuatorBase{
    public:
        void LinkServo(Servo* servo, bool is_inversed_dir);
        void SetInverseDir(bool inversed){this->__inversed_dir=inversed;};

        float GetCurrentPosition_InCncUnit() override;
        void SetPosition(float position) override;
        void MoveTo(bool is_absolute_position, float position_in_cnc_unit) override;
        float GetDistanceToTarget() override;


    protected:

    private:
        Servo* __servo;
        float __current_position_in_rad;

        bool __inversed_dir;
        float __position_offset_in_rad = 0;   // Logical position - physic position
        float __ToServoDegree(float from_cnc_rad);
        float __ToCncRad(float from_servo_angle);
};