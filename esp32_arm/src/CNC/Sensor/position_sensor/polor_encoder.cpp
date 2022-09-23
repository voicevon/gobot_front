#include "polor_encoder.h"




void PolorEncoder::LinkRawSensor(Encoder* simple_foc_encoder){
    this->__simple_foc_encoder = simple_foc_encoder;
    
}


float PolorEncoder::_GetRawValue(){
    
}
