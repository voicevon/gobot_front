#include "track_sensor_dual_ir.h"

TrackSensor_DualIR::TrackSensor_DualIR(uint8_t pin_positive_adc, uint8_t pin_negative_adc){
    this->__pin_positive_adc = pin_positive_adc;
    this->__pin_negative_adc = pin_negative_adc;
}

int16_t TrackSensor_DualIR::ReadAlignmentError(){
    return 0;
}


