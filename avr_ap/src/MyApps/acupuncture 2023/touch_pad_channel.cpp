#include "touch_pad_channel.h"
#define PIN_CAPACITY_SENSOR_COMMON 12
#define SAMPLES_COUNT_FOR_MEASURE 30
#define COPRESS_RATION 12


void TouchpadChannel::Init(uint8_t pin_sensor){
    __sensor = new CapacitiveSensor(PIN_CAPACITY_SENSOR_COMMON, pin_sensor);
    __byte_value = 0;
    __is_died = false;
}

void TouchpadChannel::Read(){
    if (__is_died){
        return;
    }
	long when_started = millis();

    long csv = __sensor->capacitiveSensor(SAMPLES_COUNT_FOR_MEASURE);   // this takes quite a 'long' time.
    if (csv == -2){
        Serial.println("This channel becomes died");
        __is_died = true;
    }else{
        __is_died = false;
        __byte_value = csv / COPRESS_RATION;
        if (__byte_value > 250)  __byte_value = 250;
        if (__byte_value < 1) __byte_value = 1;

    }

    long time_cost = millis() - when_started;
    Serial.print("time cost of this meassurement ");
    Serial.println(time_cost);
}