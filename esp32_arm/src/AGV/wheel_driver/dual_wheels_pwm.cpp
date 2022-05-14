#include "dual_wheels_pwm.h"

DualWheelsPwmDriver::DualWheelsPwmDriver(uint8_t pin_dir, uint8_t pin_pwm){
    this->__mcp23018 = nullptr;
}

DualWheelsPwmDriver::DualWheelsPwmDriver(uint8_t pin_pwm, Adafruit_MCP23X17* mcp_23018_dir_only, uint8_t pin_dir){
    this->__mcp23018 = mcp_23018_dir_only;
}

void DualWheelsPwmDriver::SetDualSpeed(float left_speed_in_percent, float right_speed_in_percent){

}