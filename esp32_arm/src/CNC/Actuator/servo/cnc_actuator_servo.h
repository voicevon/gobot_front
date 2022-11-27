#pragma once

#include "../cnc_actuator_base.h"
#include <ESP32Servo.h>


class CncActuatorServo: public CncActuatorBase{
    public:
        void LinkServo(Servo* servo, bool is_inversed_dir);

        void SpinOnce();
        void UpdateMovement(MoveBlock_SingleActuator* movement) override;

        void ForceStop() override {};   //Only G28 is using this.



    protected:

    private:
        Servo* __servo;

        float __position_offset_in_rad = 0;   // Logical position - physic position

        float __speed_degree_per_second;
        int64_t __last_spin_timestamp;
        bool __is_moving=false;
};