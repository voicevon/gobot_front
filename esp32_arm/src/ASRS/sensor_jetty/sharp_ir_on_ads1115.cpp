#include "sharp_ir_on_ads1115.h"


float JettySensor_SharpIrOnAds1115::ReadDistance(IR_POSITION irPosition){
    uint8_t pin;
    switch (irPosition){
    case IR_POSITION::LOWER_INNER:
        pin = this->__pin_lower_inner;
        break;        
    case IR_POSITION::LOWER_OUTER:
        pin = this->__pin_lower_inner;
        break;        
    case IR_POSITION::UPPER_INNER:
        pin = this->__pin_lower_inner;
        break;        
    case IR_POSITION::UPPER_OUTER:
        pin = this->__pin_lower_inner;
        break;
    default:
        break;
    }
    int16_t adc;
    float volt;
    adc = this->__ads->readADC_SingleEnded(pin);
    volt = this->__ads->computeVolts(adc);
}

void JettySensor_SharpIrOnAds1115::LinkChip(Adafruit_ADS1115* ads1115){
    this->__ads = ads1115;
}

void JettySensor_SharpIrOnAds1115::LinkPinIrPosition(IR_POSITION irPosition, uint8_t pin){
    switch (irPosition){
        case IR_POSITION::LOWER_INNER:
            this->__pin_lower_inner = pin;
            break;        
        case IR_POSITION::LOWER_OUTER:
            this->__pin_lower_inner = pin;
            break;        
        case IR_POSITION::UPPER_INNER:
            this->__pin_lower_inner = pin;
            break;        
        case IR_POSITION::UPPER_OUTER:
            this->__pin_lower_inner = pin;
            break;
    
        default:
            break;
    }
}

