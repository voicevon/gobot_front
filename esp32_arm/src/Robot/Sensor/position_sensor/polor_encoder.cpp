#include "polor_encoder.h"




void PolorEncoder::LinkRawSensor(Encoder* simple_foc_encoder){
    this->__simple_foc_encoder = simple_foc_encoder;
}


void PolorEncoder::SetCurrentPosition(float current_cnc_position){
    float raw_value = this->__simple_foc_encoder->getAngle();
    // float ranged_raw_value = this->__ModTwoPi(raw_value);
    this->__raw_offset = current_cnc_position / this->__slope_from_raw_to_cnc - raw_value ;
}

float PolorEncoder::GetCurrentPosition(){
    float raw_value = this->__simple_foc_encoder->getAngle();
    float raw_cnc_position =  (raw_value + this->__raw_offset) * this->__slope_from_raw_to_cnc;
    return this->__ModTwoPi(raw_cnc_position);
}

float PolorEncoder::GetCurrentVelocity(){
    float raw_value = this->__simple_foc_encoder->getVelocity();
    return raw_value * this->__slope_from_raw_to_cnc;
}

void PolorEncoder::SetupFormular(float slope, float raw_offset){
    this->__slope_from_raw_to_cnc = slope;
    this->__raw_offset = raw_offset;
}

float PolorEncoder::__ModTwoPi(float origin_value){
    int zoom = 100000;
    float v = (int)(origin_value * zoom) % (int)(TWO_PI * zoom);
    return v / zoom;
}
