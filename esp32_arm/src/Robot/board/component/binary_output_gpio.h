#ifndef __BINARY_OUTPUT_H_
#define __BINARY_OUTPUT_H_

#include "component_base.h"

class BinaryOutput_GPIO: public ComponentBase{ 
    public:
        BinaryOutput_GPIO(){};
        void Init(uint8_t id, uint8_t pin, uint8_t turn_on_value);

        BinaryOutput_GPIO(uint8_t id, uint8_t pin, uint8_t turn_on_value);

        void TurnOn();
        void TurnOff();
        void Invert();

        void Test(int loop_count);

    protected:
        uint8_t __pin_gpio;
        uint8_t __turn_on_value;
        uint8_t __turn_off_value;
        uint8_t __current_value;
        // BinaryOutput_GPIO(){};
};

#endif