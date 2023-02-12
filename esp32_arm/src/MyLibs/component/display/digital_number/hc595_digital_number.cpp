#include "hc595_digital_number.h"


void Hc595_Digital_number::Init(uint8_t pin_clock, uint8_t pin_data, uint8_t pin_latch){
    __pin_clock = pin_clock;
    __pin_data = pin_data;
    __pin_latch = pin_latch;
}


void Hc595_Digital_number::ShowNumber(int number){
    

}
