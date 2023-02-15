#pragma once
#include "stdint.h"


class Hc595_Digital_number{
    public:
        // Power_count:   0-9 one,  0-99 two, 0-999 three,
        void Init(uint8_t pin_clock, uint8_t pin_data, uint8_t pin_latch_mosi, uint8_t power_count);
        void ShowNumber(int number);
        // test_id = 1 : show number 1..9, to test software driver.
        // test_id = 2 : show each segments, to test led segements and hardware driver.
        void Test(int test_loop_count, int teset_id);

        void TestHardware();

    private:
        uint8_t __pin_clock;
        uint8_t __pin_data;
        uint8_t __pin_latch_mosi;
        uint8_t __power_count;

        // Define an array to translate number to 7segment
        char __segment[10] = { // segment bit pos: dp g f e d c b a
            0x3f, // zero
            0x06, // one
            0x5b, // two
            0x4f, // three
            0x66, // four
            0x6d, // five
            0x7c, // six
            0x07, // seven
            0x7f, // eight
            0x67  // nine
        };
        
        void __shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);

};