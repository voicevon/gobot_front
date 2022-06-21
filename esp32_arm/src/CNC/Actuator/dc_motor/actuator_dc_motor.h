#pragma once
#include "../actuator_base.h"
#include "SimpleFOC.h"

class ActuatorDcMotor: public ActuatorBase{
    public:
        ActuatorDcMotor(uint8_t h_bridge_pin_a, uint8_t h_bridge_pin_b);
        void LinkSensor(Sensor* sensor){this->__sensor=sensor;};
        float GetCurrentPosition_InCncUnit() override;
        void SetCurrentPositionAs(float position_in_cnc_unit) override;
        void SetTargetPositionTo(bool is_absolute_position, float position_in_cnc_unit) override;
        float GetDistanceToTarget_InCncUnit() override;
        void SetSpeed(float speed_per_second) override;
        void Stop() override;

        void SpinOnce();
        void StartToMove();
        bool IsMoving(){return this->__is_moving;};

    private:
        uint8_t __h_bridge_pin_a;
        uint8_t __h_bridge_pin_b;

        bool __is_moving = false;

        Sensor* __sensor;
};