#ifndef _BOARD_BASE_H_
#define _BOARD_BASE_H_

#include <Arduino.h>


template<int LED_COUNT>
class BoardBase{
    public:
        BoardBase();
        void AppendSingleLed(uint8_t index, uint8_t pinNumber, uint8_t turn_on_level);
        void TurnOn_SingleLed(uint8_t index);
        void TurnOff_SingleLed(uint8_t index);
        void TurnOn_AllLeds();
        void TurnOff_AllLeds();
        void Flash_AllLeds(uint8_t count, uint16_t on_ms, uint16_t off_ms);

    private:
        // uint8_t __leds_count;
        uint8_t leds_pin[LED_COUNT];
        uint8_t leds_on_level[LED_COUNT];
        // uint8_t led_count = 0;
};

#endif