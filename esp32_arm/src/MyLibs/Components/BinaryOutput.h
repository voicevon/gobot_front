#ifndef __BINARY_OUTPUT_H_
#define __BINARY_OUTPUT_H_

#include "ComponentBase.h"

class BinaryOutput: public ComponentBase{ 
    public:
        BinaryOutput(uint8_t id, uint8_t pin, uint8_t turn_on_level);
        void TurnOn(){};
        void TurnOff(){};

    protected:
        BinaryOutput(){};
};

#endif