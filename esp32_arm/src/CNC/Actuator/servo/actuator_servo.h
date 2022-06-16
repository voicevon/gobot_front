#pragma once

#include "../actuator_base.h"
#include <ESP32Servo.h>


class ActuatorServo: public ActuatorBase{
    public:
        void LinkServo(Servo* servo){this->__servo=servo;};
        float GetCurrentPosition() override;
        void SetPosition(float position) override;
        void MoveTo(bool is_absolute_position, float position) override;
        void SetInverseDir(bool inversed){this->__inversed_dir = inversed};

    protected:

    private:
        Servo* __servo;
        float __current_position_in_rad;
        float __position_offset_in_rad = 0;   // Logical position - physic position
        bool __inversed_dir = false;

        float __ToServoDegree(float from_cnc_rad);
        float __ToCncRad(float from_servo_angle);
};