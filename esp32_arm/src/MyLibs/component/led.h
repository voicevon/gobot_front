#pragma once

#include "binary_output_gpio.h"


class Led: public BinaryOutput_GPIO
{
    public:
        Led(uint8_t id, uint8_t pin, uint8_t turn_on_level):BinaryOutput_GPIO(id, pin, turn_on_level){};
        void Flash(){};
    protected:

    private:

};

