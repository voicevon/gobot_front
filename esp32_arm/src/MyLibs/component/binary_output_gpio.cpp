#include "binary_output_gpio.h"

BinaryOutput_GPIO::BinaryOutput_GPIO(uint8_t id, uint8_t pin, uint8_t turn_on_value){
    _id = id;
    __pin_gpio = pin;
    pinMode(pin, OUTPUT);
    __turn_on_value = turn_on_value;

    __turn_off_value = HIGH;
    if (turn_on_value == HIGH) __turn_off_value = LOW;

}

void BinaryOutput_GPIO::TurnOn(){
    digitalWrite(__pin_gpio, __turn_on_value);
}

void BinaryOutput_GPIO::TurnOff(){
    digitalWrite(__pin_gpio, __turn_off_value);
}
