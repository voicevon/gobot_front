#include "single_wheel_h_bridge_pwm_driver.h"
#include "MyLibs/board_base.h"



SingleWheel_HBridgePwmDriver::SingleWheel_HBridgePwmDriver(uint8_t pin_dir, uint8_t pin_pwm){
    pinMode(pin_dir, OUTPUT);
    pinMode(pin_pwm, OUTPUT);

    this->__mcp23018 = nullptr;
    this->__pin_dir = pin_dir;
    this->__pin_pwm = pin_pwm;


    BoardBase board;
    this->__ledc_channel = board.Assign_ledc_channel();
    const int freq = 5000;
    const int resolution = 8;
    ledcSetup(this->__ledc_channel, freq, resolution);
    ledcAttachPin(pin_pwm, this->__ledc_channel);


}

SingleWheel_HBridgePwmDriver::SingleWheel_HBridgePwmDriver(uint8_t pin_pwm, Adafruit_MCP23X17* mcp_23018_dir_only, uint8_t pin_dir){
    pinMode(pin_pwm, OUTPUT);
    mcp_23018_dir_only->pinMode(pin_dir, OUTPUT);
    this->__mcp23018 = mcp_23018_dir_only;
    this->__pin_dir = pin_dir;
    this->__pin_pwm = pin_pwm;                                                                                                                                                                                                                                                                                                                                                                                                          
}

void SingleWheel_HBridgePwmDriver::SetVelocity_in_percent(int8_t velocity){
    // Serial.print("[debug] SingleWheel_HBridgePwmDriver::SetVelocity_in_percent()  velocity = ");
    // Serial.println(velocity);
    
    uint32_t dutycycle = 2.55f * velocity;
    if (velocity == 0){
        digitalWrite(this->__pin_dir, LOW);
        digitalWrite(this->__pin_pwm, LOW);
    }else if(velocity > 0){
        digitalWrite(this->__pin_dir, HIGH);
        ledcWrite(this->__ledc_channel, dutycycle);
    }else{
        digitalWrite(this->__pin_dir, LOW);
        ledcWrite(this->__ledc_channel, 255-dutycycle);
    }

}
