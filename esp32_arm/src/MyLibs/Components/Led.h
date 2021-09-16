#ifndef __LED_H_
#define __LED_H_

#include "ComponentBase.h"
class Led: public ComponentBase{
    public:
        Led(uint8_t id, uint8_t pin, uint8_t turn_on_level);
        void TurnOn(){};
        void TurnOff(){};
        void Flash(){};
    protected:

    private:

};

#endif
