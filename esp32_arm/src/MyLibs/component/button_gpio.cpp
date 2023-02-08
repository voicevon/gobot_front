#include "button_gpio.h"
#include "Arduino.h"

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
        if (__callback != nullptr){
            bool go_on_remote = true;  // TODO:   *__callback();
            // *__callback();
            if (go_on_remote){
                if(_mqtt_topic != nullptr){
                    if(is_pressed) {
                        _PublishValue(_PAYLOAD_STRING_ON.c_str());
                    }else{
                        _PublishValue(_PAYLOAD_STRING_OFF.c_str());
                    }
                }
            }
        }
    } 
    __last_state_is_pressed = is_pressed;
}


bool Button_Gpio::IsPressed(){
    SpinOnce();
    return __last_state_is_pressed;
}