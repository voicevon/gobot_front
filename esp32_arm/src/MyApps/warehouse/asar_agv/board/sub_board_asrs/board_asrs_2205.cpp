#include "board_asrs_2205.h"

void BoardAsrs::Init(){

}

void BoardAsrs::LinkTwoWireBus(TwoWire* i2c_bus){
    if(! this->__ads1115.begin(I2C_ADDR_ADS1115, i2c_bus)){
        Serial.println("Getting single-ended readings from AIN0..3");
        Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");

        // The ADC input range (or gain) can be changed via the following
        // functions, but be careful never to exceed VDD +0.3V max, or to
        // exceed the upper and lower limits if you adjust the input range!
        // Setting these values incorrectly may destroy your ADC!
        //                                                                ADS1015  ADS1115
        //                                                                -------  -------
        // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
        // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
        // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
        // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
        // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
        // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
        Serial.println("[Error] BoardAsrs::Init()   Failed to initialize ADS.");
        while (1);
    }
    this->__jettySensor.LinkChip(&this->__ads1115);
    this->__jettySensor.LinkPinIrPosition(JettySensorBase::IR_POSITION::LOWER_INNER, ADS1115_PIN_LOWER_INNER);
    this->__jettySensor.LinkPinIrPosition(JettySensorBase::IR_POSITION::LOWER_OUTER, ADS1115_PIN_LOWER_OUTER);
    this->__jettySensor.LinkPinIrPosition(JettySensorBase::IR_POSITION::UPPER_INNER, ADS1115_PIN_UPPER_INNER);
    this->__jettySensor.LinkPinIrPosition(JettySensorBase::IR_POSITION::UPPER_OUTER, ADS1115_PIN_UPPER_OUTER);
    Serial.println("[Info] BoardAsrs::LinkTwoWireBus() is OK.");
    // , ADS1115_PIN_UPPER_OUTER, ADS1115_PIN_LOWER_INNER, ADS1115_PIN_LOWER_OUTER);
}


