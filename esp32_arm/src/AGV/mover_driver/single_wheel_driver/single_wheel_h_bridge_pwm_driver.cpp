#include "single_wheel_h_bridge_pwm_driver.h"



SingleWheel_HBridgePwmDriver::SingleWheel_HBridgePwmDriver(uint8_t pin_dir, uint8_t pin_pwm){
    this->__mcp23018 = nullptr;
    this->__pin_dir = pin_dir;
    this->__pin_pwm = pin_pwm;
}

SingleWheel_HBridgePwmDriver::SingleWheel_HBridgePwmDriver(uint8_t pin_pwm, Adafruit_MCP23X17* mcp_23018_dir_only, uint8_t pin_dir){
    this->__mcp23018 = mcp_23018_dir_only;
    this->__pin_dir = pin_dir;
    this->__pin_pwm = pin_pwm;                                                                                                                                                                                                                                                                                                                                                                                                          
}