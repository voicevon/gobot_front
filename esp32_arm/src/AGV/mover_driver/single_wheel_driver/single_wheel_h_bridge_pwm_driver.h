#pragma once
#include <Adafruit_MCP23X17.h>
#include"single_wheel_driver_base.h"


class SingleWheel_HBridgePwmDriver: public SingleWheelDriverBase{
    public:
        SingleWheel_HBridgePwmDriver(uint8_t pin_dir, uint8_t pin_pwm);
        SingleWheel_HBridgePwmDriver(uint8_t pin_pwm, Adafruit_MCP23X17* mcp_23018_dir_only, uint8_t pin_dir);
        void SetVelocity_in_percent(int8_t velocity) override;


    private:
        Adafruit_MCP23X17* __mcp23018;
        uint8_t __pin_pwm;
        uint8_t __pin_dir;
};
