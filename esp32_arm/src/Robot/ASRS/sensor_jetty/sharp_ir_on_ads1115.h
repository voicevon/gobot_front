#pragma once


#include "jetty_sensor_base.h"
#include "Adafruit_ADS1X15.h"



class JettySensor_SharpIrOnAds1115: public JettySensorBase{
    public:
        JettySensor_SharpIrOnAds1115(){};
        float ReadDistance(IR_POSITION irPosition) override;

        void LinkChip(Adafruit_ADS1115* ads1115);
        void LinkPinIrPosition(IR_POSITION position, uint8_t pin);

    private:
        Adafruit_ADS1115* __ads;  /* Use this for the 16-bit version */
        uint8_t __pin_upper_inner;
        uint8_t __pin_upper_outer;
        uint8_t __pin_lower_inner;
        uint8_t __pin_lower_outer;

};
