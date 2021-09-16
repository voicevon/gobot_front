#include "boardbase.h"

template<int LEDS_ON_BOARD>
void BoardBase<LEDS_ON_BOARD>::AppendSingleLed(uint8_t index, uint8_t pinNumber, uint8_t turn_on_level){
    if (index >=LEDS_ON_BOARD){
        Serial.println("ERR   BoardBase::AppendSingleLed(), Index is out of range");
        return;
    }
    leds_pin[index] = pinNumber;
    leds_on_level[index] = turn_on_level;
    pinMode(pinNumber, OUTPUT);
    TurnOn_SingleLed(index);
}

template<int LED_COUNT>
void BoardBase<LED_COUNT>::TurnOn_SingleLed(uint8_t index){
    if (leds_pin !=0)
        digitalWrite(leds_pin[index], leds_on_level[index]);
}

template<int LED_COUNT>
void BoardBase<LED_COUNT>::TurnOff_SingleLed(uint8_t index){
    if (leds_pin !=0)
        digitalWrite(leds_pin[index], !leds_on_level[index]);
}

template<int LED_COUNT>
void BoardBase<LED_COUNT>::TurnOn_AllLeds(){
    for(int i=0; i<LED_COUNT; i++){
        TurnOn_SingleLed(i);
    }
}

template<int LED_COUNT>
void BoardBase<LED_COUNT>::TurnOff_AllLeds(){
    for(int i=0; i<LED_COUNT; i++){
        TurnOff_SingleLed(i);
    }

}

template<int I>
void BoardBase<I>::Flash_AllLeds(uint8_t onoff_count, uint16_t on_ms, uint16_t off_ms){
    for (int i=0; i< onoff_count; i++){
        TurnOn_AllLeds();
        delay(on_ms);
        TurnOff_AllLeds();
        delay(off_ms);
    }
}

