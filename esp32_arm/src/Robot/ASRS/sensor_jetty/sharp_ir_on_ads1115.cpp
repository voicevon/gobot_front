#include "sharp_ir_on_ads1115.h"


float JettySensor_SharpIrOnAds1115::ReadDistance(IR_POSITION irPosition){
    uint8_t pin = 0;
    switch (irPosition){
    case IR_POSITION::LOWER_INNER:
        pin = this->__pin_lower_inner;
        break;        
    case IR_POSITION::LOWER_OUTER:
        pin = this->__pin_lower_outer;
        break;        
    case IR_POSITION::UPPER_INNER:
        pin = this->__pin_upper_inner;
        break;        
    case IR_POSITION::UPPER_OUTER:
        pin = this->__pin_upper_outer;
        break;
    default:
        break;
    }
    int16_t adc;
    float volt;

    adc = this->__ads->readADC_SingleEnded(pin);
    volt = this->__ads->computeVolts(adc);
     // eqution of the fitting curve
    ////33.9 + -69.5x + 62.3x^2 + -25.4x^3 + 3.83x^4
    float distance_in_cm =  33.9 + -69.5*(volt) + 62.3*pow(volt,2) + -25.4*pow(volt,3) + 3.83*pow(volt,4);
    bool debug = false;
    if (debug){
        Serial.print("\t\t\t\t\t\t");
        Serial.print(pin);
        Serial.print("\t");
        Serial.print(adc);
        Serial.print("\t");
        Serial.print(volt);
        Serial.print("\t");
        Serial.println(distance_in_cm);
    }
    return distance_in_cm * 10;
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
            this->__pin_lower_outer = pin;
            break;        
        case IR_POSITION::UPPER_INNER:
            this->__pin_upper_inner = pin;
            break;        
        case IR_POSITION::UPPER_OUTER:
            this->__pin_upper_outer = pin;
            break;
    
        default:
            break;
    }
}

