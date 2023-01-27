#pragma once

#include "binary_output_gpio.h"


class Led_GPIO: public BinaryOutput_GPIO
{
    public:
        Led_GPIO():BinaryOutput_GPIO(){};   // Is this necessary?
        Led_GPIO(uint8_t id, uint8_t pin, uint8_t turn_on_level):BinaryOutput_GPIO(id, pin, turn_on_level){};
        void Flash();
    protected:

    private:

};

