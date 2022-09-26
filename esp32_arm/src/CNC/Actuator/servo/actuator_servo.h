#pragma once

#include "../actuator_base.h"
#include <ESP32Servo.h>


class ActuatorServo: public ActuatorBase{
    public:
        void LinkServo(Servo* servo, bool is_inversed_dir);
        void SetInverseDir(bool inversed){this->__inversed_dir=inversed;};

        void SpinOnce();
        bool IsMoving(){return this->__is_moving;};

        // void Stop() override {this->__is_moving=false;};
        void UpdateTargetPositionFromCurrent() override {this->__is_moving=false;};
        void StartToMove();

        void SetCurrentPositionAs(float cnc_position_in_rad) override;
        // void SetTargetPositionTo(bool is_absolute_position, float position_in_cnc_unit) override;
        void UpdateMovement(LineSegment* movement) override;

        float GetAbsDistanceToTarget_InCncUnit() override;
        void SetSpeed(float speed_in_cnc_unit) override;   // There is no "unsigned float"
        virtual void SetAccelleration(float accelleration_in_cnc_unit){}; 



    protected:
        float GetSpeed() override {return DEG_TO_RAD * this->__speed_degree_per_second;};

    private:
        Servo* __servo;

        bool __inversed_dir;
        float __position_offset_in_rad = 0;   // Logical position - physic position
        float __ToServoDegree(float from_cnc_rad);
        float __ToCncRad(float from_servo_angle);

        float __speed_degree_per_second;
        int64_t __last_spin_timestamp;
        int8_t __moving_direction_of_cnc = 1 ;
        bool __is_moving=false;
        // float __distance_to_target_in_rad;
};