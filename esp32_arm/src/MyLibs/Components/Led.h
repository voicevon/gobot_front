#ifndef __LED_H_
#define __LED_H_

// #include "ComponentBase.h"
#include "BinaryOutput.h"


class Led: public BinaryOutput{
    public:
        Led(uint8_t id, uint8_t pin, uint8_t turn_on_level);
        void Flash(){};
    protected:

    private:

};

#endif
