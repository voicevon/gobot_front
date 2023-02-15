#include "hc595_digital_number.h"
#include "MyLibs/basic/logger.h"
#include "Arduino.h"   // for digitalWrite()

void Hc595_Digital_number::Init(uint8_t pin_clock, uint8_t pin_data, uint8_t pin_latch_mosi, uint8_t power_count){
    __pin_clock = pin_clock;
    __pin_data = pin_data;
    __pin_latch_mosi = pin_latch_mosi;
    __power_count = power_count;
    pinMode(pin_clock, OUTPUT);
    pinMode(pin_data, OUTPUT);
    pinMode(pin_latch_mosi, OUTPUT);
}


void Hc595_Digital_number::ShowNumber(int number){
    // https://www.best-microcontroller-projects.com/74hc595.html
    unsigned char left_number,right_num;
    left_number = number / 10;
    right_num = number % 10;

    digitalWrite(__pin_latch_mosi, LOW);
    shiftOut(__pin_data, __pin_clock, MSBFIRST, __segment[left_number]);
    shiftOut(__pin_data, __pin_clock, MSBFIRST, __segment[right_num]);
    // shiftOut(__pin_data, __pin_clock, MSBFIRST, (0xff00 & __data)>>8);   //???
    // shiftOut(__pin_data, __pin_clock, MSBFIRST, 0x00ff & __data);
    digitalWrite(__pin_latch_mosi, HIGH);

    // Serial.println(ltoa(__data, buf, 16));
    // ???
    // if (!dir) __data<<=1; else __data>>=1; // Shift

    // if (__data & 0x8000) dir=1;           // Set direction.
    // if (__data & 0x1) dir=0;

    // delay(200);
}

void Hc595_Digital_number::Test(int test_loop_count, int teset_id){
    uint8_t test_bit = 0;
    switch (teset_id) {
    case 1:
        for (int i=0; i<test_loop_count; i++){
            for (int x=0; x<10; x++){
                for(int power=0; x<__power_count; power++){
                    ShowNumber(x * pow10(power));
                    delay(500);
                }
            }
        }
        break;
    case 2:
        Logger::Debug("testing digital number leds hardware");
        for (int i=0; i<test_loop_count; i++){
            for (int x=0; x<10; x++){
                digitalWrite(__pin_latch_mosi, LOW);
                for(int power=0; x<__power_count; power++){
                    for(int segment_bit=0; segment_bit<8; segment_bit++){
                        if (test_bit == segment_bit){
                            digitalWrite(__pin_data, LOW);
                            Logger::Print("segment_bit  LOW", segment_bit);
                        }else{
                            digitalWrite(__pin_data, HIGH);
                            Logger::Print("segment_bit  HIGH", segment_bit);
                        }

                        delay(1);
                        digitalWrite(__pin_clock, LOW);
                        delay(1);
                        digitalWrite(__pin_clock, HIGH);
                        delay(1);

                        delay(500);
                    }
                    test_bit++;
                    if (test_bit >=8) test_bit = 0;
                }
                digitalWrite(__pin_latch_mosi, HIGH);
            }
        }        
        break;
    default:
        Logger::Warn("Hc595_Digital_number::Test() ");
        Logger::Print("Unsupported test_id", teset_id);
        break;
    }

}



void Hc595_Digital_number::__shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
    uint8_t i;

    for(i = 0; i < 8; i++) {
        if(bitOrder == LSBFIRST)
            digitalWrite(dataPin, !!(val & (1 << i)));
        else
            digitalWrite(dataPin, !!(val & (1 << (7 - i))));
        delay(1);
        digitalWrite(clockPin, HIGH);
        delay(1);
        digitalWrite(clockPin, LOW);
        delay(1);
    }
}




