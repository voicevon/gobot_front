#include "button_gpio.h"
#include "Arduino.h"
#include "MyLibs/basic/my_const.h"

Button_Gpio::Button_Gpio(int gpio_id){
    __pin_number = gpio_id;
    pinMode(__pin_number, INPUT_PULLUP);
}
void Button_Gpio::Init_Remotable( char* mqtt_topic){
    _mqtt_topic =  mqtt_topic;
}

void Button_Gpio::SpinOnce(){
    bool is_pressed = ! digitalRead(__pin_number);
    if (is_pressed && (!__last_state_is_pressed)){
        if(_mqtt_topic != nullptr){
            if(is_pressed) {
                _PublishValue(PAYLOAD_STRING_ON);
            }else{
                _PublishValue(PAYLOAD_STRING_OFF);
            }
        }
    } 
    __last_state_is_pressed = is_pressed;
}


bool Button_Gpio::IsPressed(){
    SpinOnce();
    return __last_state_is_pressed;
}