#pragma once
#include "../actuator_base.h"
#include "SimpleFOC.h"

class   ActuatorDcMotor: public ActuatorBase{
    public:
        ActuatorDcMotor(uint8_t h_bridge_pin_dir, uint8_t h_bridge_pin_speed);
        void PrintOut();
        void LinkSensor(Sensor* sensor){this->__sensor=sensor;};
        // void LinkPidController(PIDController* pid){this->__pid=pid;};
        float GetCurrentPosition_InCncUnit() override;
        void SetCurrentPositionAs(float position_in_cnc_unit) override;
        void SetTargetPositionTo(bool is_absolute_position, float position_in_cnc_unit) override;
        float GetAbsDistanceToTarget_InCncUnit() override;
        void SetSpeed(float speed_in_cnc_unit) override;   // When motor is running, should not effect running speed.

        void Stop() override;
        float GetSpeed() override {return this->__cnc_speed;};
        void SetAccelleration(float accelleration_in_cnc_unit) override;

        void SpinOnce();

        void SetPwmSpeed(bool dir_is_cw,  uint32_t pwm_speed);
        bool IsMoving(){return this->__is_moving;};

    private:
        uint8_t __h_bridge_pin_dir;
        uint8_t __h_bridge_pin_speed;
        uint8_t __pwm_channel;
        
        // float __sensor_rad_per_mm = 1.0f;
        float __sensor_offset = 0.0f;
        Sensor* __sensor;

        // PIDController* __pid;

        // uint __pwm_speed;
        float __cnc_speed;   // mm/s  or   rad/s
        bool __is_moving = false;
        int SENSOR_GEAR_COUNT = 10;
        int CHAIN_PITCH_COUNT = 157;

        float __SLOPE_FROM_CNC_TO_SENSOR = 1.0;
        float __SLOPE_FROM_SENSOR_TO_CNC = 1.0;

        int __count_down = 0;
        LowPassFilter __filter = LowPassFilter(0.05f);
        PIDController __speed_pid = PIDController(1.0f, 1.0f, 0.0f, 10.0f, 255.0f);
        // PIDController __speed_pid {1.0f, 1.0f, 0.0f, 10.0f, 100.0f};



};