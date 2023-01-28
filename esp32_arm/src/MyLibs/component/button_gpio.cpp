#include "button_gpio.h"
#include "Arduino.h"


Button_Gpio::Button_Gpio(int gpio_id){
    __pin_number = gpio_id;
    pinMode(__pin_number, INPUT_PULLUP);
}

void Button_Gpio::SpinOnce(){
    bool is_pressed = ! digitalRead(__pin_number);
    if (is_pressed && (!__last_state_is_pressed)){
        __is_changing_to_pressed = true;
    } 
    __last_state_is_pressed = is_pressed;
}


bool Button_Gpio::IsPressed(){

}