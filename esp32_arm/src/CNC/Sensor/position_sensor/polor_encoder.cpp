#include "polor_encoder.h"




void PolorEncoder::LinkRawSensor(Encoder* simple_foc_encoder){
    this->__simple_foc_encoder = simple_foc_encoder;
    
}


void PolorEncoder::SetCurrentPosition(float current_cnc_position){
    this->__raw_offset = 0;
    this->__slope = 1;
}

float PolorEncoder::GetCurrentPosition(){
    float raw_value = this->__simple_foc_encoder->getAngle();
    return (raw_value + this->__raw_offset) * this->__slope;
}

void PolorEncoder::SetupFormular(float slope, float raw_offset){
    this->__slope = slope;
    this->__raw_offset = raw_offset;
}


// float PolorEncoder::_GetRawValue(){
    
// }
