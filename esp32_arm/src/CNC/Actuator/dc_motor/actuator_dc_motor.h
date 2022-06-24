#pragma once
#include "../actuator_base.h"
#include "SimpleFOC.h"

class ActuatorDcMotor: public ActuatorBase{
    public:
        ActuatorDcMotor(uint8_t h_bridge_pin_a, uint8_t h_bridge_pin_b);
        void LinkSensor(Sensor* sensor){this->__sensor=sensor;};
        // void LinkPidController(PIDController* pid){this->__pid=pid;};
        float GetCurrentPosition_InCncUnit() override;
        void SetCurrentPositionAs(float position_in_cnc_unit) override;
        void SetTargetPositionTo(bool is_absolute_position, float position_in_cnc_unit) override;
        float GetAbsDistanceToTarget_InCncUnit() override;
        void SetSpeed(float speed_per_second) override;   // When motor is running, should not effect running speed.
        void UpdateSpeedWhenMotorIsRunning(float new_speed);      
        void Stop() override;
        // void SetDirection(bool is_to_CW);
        float GetSpeed() override {return this->__speed;};

        void SpinOnce();
        void StartToMove();
        bool IsMoving(){return this->__is_moving;};

    private:
        uint8_t __h_bridge_pin_a;
        uint8_t __h_bridge_pin_b;
        float __sensor_rad_per_mm = 1.0f;
        float __offset = 0.0f;
        uint8_t __pwm_channel_a;
        uint8_t __pwm_channel_b;
        Sensor* __sensor;

        // PIDController* __pid;

        uint __speed;
        bool __is_moving = false;




};