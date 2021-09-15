#ifndef BOARD_BASE_H_
#define BOARD_BASE_H_

#include <Arduino.h>

class BoardBase{
    public:
        BoardBase();
        void InitLed(uint8_t index, uint8_t pinNumber, uint8_t turn_on_level);
        void TurnOn_SingleLed(uint8_t index);
        void TurnOff_SingleLed(uint8_t index);
        void TurnOn_AllLed();
        void TurnOff_AllLed();
};

#endif