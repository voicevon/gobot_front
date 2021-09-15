#include "boardbase.h"

BoardBase::BoardBase(){
    for(int i =0; i<5; i++){
        leds_pin[i] = 0;
    }
}
void BoardBase::AppendSingleLed(uint8_t index, uint8_t pinNumber, uint8_t turn_on_level){
    if (index >=5){
        Serial.println("ERR   BoardBase::AppendSingleLed(), Index is out of range");
        return;
    }
    leds_pin[index] = pinNumber;
    leds_on_level[index] = turn_on_level;
    pinMode(pinNumber, OUTPUT);
    TurnOn_SingleLed(index);
}
void BoardBase::TurnOn_SingleLed(uint8_t index){
    if (leds_pin !=0)
        digitalWrite(leds_pin[index], leds_on_level[index]);
}
void BoardBase::TurnOff_SingleLed(uint8_t index){
    if (leds_pin !=0)
        digitalWrite(leds_pin[index], !leds_on_level[index]);
}
void BoardBase::TurnOn_AllLeds(){
    for(int i=0; i<5; i++){
        TurnOn_SingleLed(i);
    }
}
void BoardBase::TurnOff_AllLeds(){
    for(int i=0; i<5; i++){
        TurnOff_SingleLed(i);
    }

}

void BoardBase::Flash_AllLeds(uint8_t count, uint16_t on_ms, uint16_t off_ms){
    for (int i=0; i< count; i++){
        TurnOn_AllLeds();
        delay(on_ms);
        TurnOff_AllLeds();
        delay(off_ms);
    }
}

