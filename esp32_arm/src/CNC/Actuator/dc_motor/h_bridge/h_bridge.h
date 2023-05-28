#pragma once

#include <stdint.h>

class H_Bridge{

    public:
        H_Bridge(){};
        H_Bridge(uint8_t h_bridge_pin_dir, uint8_t h_bridge_pin_pwm_speed);
        void Init(uint8_t h_bridge_pin_dir, uint8_t h_bridge_pin_pwm_speed);
        void SetPwmSpeed(bool dir_is_cw,  uint32_t pwm_speed);
        void Stop();

    private:
        uint8_t __h_bridge_pin_dir;
        uint8_t __h_bridge_pin_pwm_speed;
        uint8_t __pwm_channel;

};