#include "gpio_button.h"
#include "Arduino.h"


GpioButton::GpioButton(int gpio_id){
    __pin_number = gpio_id;
    pinMode(__pin_number, INPUT_PULLUP);
}

void GpioButton::SpinOnce(){
    bool is_pressed = ! digitalRead(__pin_number);
    if (IsPressed && (!__last_state_is_pressed)){
        __is_changing_to_pressed = true;
    } 
    __last_state_is_pressed = is_pressed;
}


bool GpioButton::IsPressed(){

}