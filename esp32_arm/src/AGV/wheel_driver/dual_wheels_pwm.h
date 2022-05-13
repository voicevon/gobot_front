#pragma once

#include "wheel_driver_base.h"
#include <Adafruit_MCP23X17.h>

class DualWheelsPwmDriver: public WheelDriver{
    public:
        DualWheelsPwmDriver(uint8_t pin_dir, uint8_t pin_pwm);
        DualWheelsPwmDriver(Adafruit_MCP23X17* mcp_23018_dir_only, uint8_t pin_dir, uint8_t pin_pwm);
        
        // void SetSingleSpeed(int speed_in_percent) override;
        void SetDualSpeed(float left_speed_in_percent, float right_speed_in_percent) override;

    private:
        Adafruit_MCP23X17* __mcp23018;
};