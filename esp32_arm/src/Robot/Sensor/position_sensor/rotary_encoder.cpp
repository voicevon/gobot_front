#include "rotary_encoder.h"




void RotaryEncoder::LinkRawSensor(Encoder* simple_foc_encoder){
    this->__simple_foc_encoder = simple_foc_encoder;
}


void RotaryEncoder::SetCurrentPosition(float current_cnc_position){
    float raw_value = this->__simple_foc_encoder->getAngle();
    // float ranged_raw_value = this->__ModTwoPi(raw_value);
    this->__formular_raw_offset = current_cnc_position / this->__formular_slope_from_raw_to_cnc - raw_value ;
}

float RotaryEncoder::GetCurrentPosition(){
    float raw_value = this->__simple_foc_encoder->getAngle();
    float raw_cnc_position =  (raw_value + this->__formular_raw_offset) * this->__formular_slope_from_raw_to_cnc;
    return this->__ModTwoPi(raw_cnc_position);
}

float RotaryEncoder::GetCurrentVelocity(){
    float raw_value = this->__simple_foc_encoder->getVelocity();
    return raw_value * this->__formular_slope_from_raw_to_cnc;
}

void RotaryEncoder::SetupFormular(float slope, float raw_offset){
    this->__formular_slope_from_raw_to_cnc = slope;
    this->__formular_raw_offset = raw_offset;
}

float RotaryEncoder::__ModTwoPi(float origin_value){
    int zoom = 100000;
    float v = (int)(origin_value * zoom) % (int)(TWO_PI * zoom);
    return v / zoom;
}
