#pragma once
#include "stdint.h"


class Hc595_Digital_number{
    public:
        void Init(uint8_t pin_clock, uint8_t pin_data, uint8_t pin_latch);
        void ShowNumber(int number);
    private:
        uint8_t __pin_clock;
        uint8_t __pin_data;
        uint8_t __pin_latch;

};