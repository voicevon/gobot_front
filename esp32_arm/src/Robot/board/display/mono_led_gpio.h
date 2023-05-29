#pragma once

// #include "../binary_output_gpio.h"
#include "Robot/board/component/binary_output_gpio.h"



class Mono_Led_GPIO: public BinaryOutput_GPIO
{
    public:
        Mono_Led_GPIO():BinaryOutput_GPIO(){};   // Is this necessary?
        Mono_Led_GPIO(uint8_t id, uint8_t pin, uint8_t turn_on_level):BinaryOutput_GPIO(id, pin, turn_on_level){};
        void Flash();
    protected:

    private:

};

