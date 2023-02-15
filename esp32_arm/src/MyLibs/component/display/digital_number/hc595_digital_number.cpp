#include "hc595_digital_number.h"
#include "MyLibs/basic/logger.h"
#include "Arduino.h"   // for digitalWrite()

void Hc595_Digital_number::Init(uint8_t pin_clock, uint8_t pin_data, uint8_t pin_latch_mosi, uint8_t power_count){
    __pin_clock = pin_clock;
    __pin_data = pin_data;
    __pin_latch_mosi = pin_latch_mosi;
    __power_count = power_count;
    pinMode(__pin_clock, OUTPUT);
    pinMode(__pin_data, OUTPUT);
    pinMode(__pin_latch_mosi, OUTPUT);
}


void Hc595_Digital_number::ShowNumber(int number){
    // https://www.best-microcontroller-projects.com/74hc595.html
    unsigned char left_number,right_num;
    left_number = number / 10;
    right_num = number % 10;

    digitalWrite(__pin_latch_mosi, LOW);
    shiftOut(__pin_data, __pin_clock, MSBFIRST, 255 - __segment[right_num]);
    shiftOut(__pin_data, __pin_clock, MSBFIRST, 255 - __segment[left_number]);
    digitalWrite(__pin_latch_mosi, HIGH);

    // Serial.println(ltoa(__data, buf, 16));
    // ???
    // if (!dir) __data<<=1; else __data>>=1; // Shift

    // if (__data & 0x8000) dir=1;           // Set direction.
    // if (__data & 0x1) dir=0;

    // delay(200);
}

void Hc595_Digital_number::Test(int test_loop_count, int teset_id){

        unsigned char smgduan[] = {  //共阳
            B11000000, // 0
            B11111001, // 1
            B10100100, // 2
            B10110000, // 3
            B10011001, // 3
            B10010010, // 8
            B10000010, // 6
            B11111000, // 7
            B10000000, // 8
            B10010000, // 9
            };//显示0~9的值共阴


    uint8_t test_value = 0;
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
        pinMode(__pin_clock, OUTPUT);
        pinMode(__pin_data, OUTPUT);
        pinMode(__pin_latch_mosi, OUTPUT);

        for (int i=0; i<test_loop_count; i++){
            Logger::Debug("new test loop....");
            Logger::Print("__pin_data", __pin_data);
            Logger::Print("__pin_clock", __pin_clock);
            Logger::Print("__pin_latch_mosi", __pin_latch_mosi);

            for(int segment_bit=0; segment_bit<8; segment_bit++){
                Logger::Debug("new segment");
                test_value = 255 - (1<< segment_bit);
                // test_value = B11000000;
                Logger::Print("segment_bit", segment_bit);
                Logger::Print("test_value", test_value);
                digitalWrite(__pin_latch_mosi, LOW);
                __shiftOut(__pin_data,  __pin_clock, MSBFIRST, test_value);
                __shiftOut(__pin_data,  __pin_clock, MSBFIRST, test_value);
                digitalWrite(__pin_latch_mosi, HIGH);
                delay(1000);
            }
            // test_bit++;
            // if (test_bit >=8) test_bit = 0;

        }     
        break;

    case 3:
        for(int i=0; i<9999; i++) {  
            Logger::Print("i", i);
            pinMode(__pin_clock, OUTPUT);
            pinMode(__pin_data, OUTPUT);
            pinMode(__pin_latch_mosi, OUTPUT);
            unsigned char gewei = (i%100)%10;
            unsigned char shiwei = (i%100)/10;
            // unsigned char baiwei = (i%1000)/100; 
            // unsigned char qianwei = i/1000; 
            digitalWrite(__pin_latch_mosi,LOW); //低电位表示启动
            __shiftOut(__pin_data,__pin_clock,MSBFIRST,smgduan[gewei]);
            __shiftOut(__pin_data,__pin_clock,MSBFIRST,smgduan[shiwei]);
            // shiftOut(__pin_data,__pin_clock,MSBFIRST,smgduan[baiwei]);
            // shiftOut(__pin_data,__pin_clock,MSBFIRST,smgduan[qianwei]);
            digitalWrite(__pin_latch_mosi,HIGH); //高电位表示停止
            delayMicroseconds(2);
            digitalWrite(__pin_latch_mosi, HIGH);//ST_CP
            delay(100);
        }
        break;
    default:
        Logger::Warn("Hc595_Digital_number::Test() ");
        Logger::Print("Unsupported test_id", teset_id);
        break;
    }

}

void Hc595_Digital_number::TestHardware(){
    uint8_t test_value;
    Logger::Debug("Hc595_Digital_number::HardwareTest()");
    // pinMode(__pin_clock, OUTPUT);
    // pinMode(__pin_data, OUTPUT);
    // pinMode(__pin_latch_mosi, OUTPUT);

    for (int i=0; i<99; i++){
        Logger::Debug("new test loop....");
        // Logger::Print("__pin_data", __pin_data);
        // Logger::Print("__pin_clock", __pin_clock);
        // Logger::Print("__pin_latch_mosi", __pin_latch_mosi);

        for(int segment_bit=0; segment_bit<8; segment_bit++){
            Logger::Debug("new segment");
            test_value = 255 - (1<< segment_bit);
            // test_value = B11000000;
            // Logger::Print("segment_bit", segment_bit);
            // Logger::Print("test_value", test_value);
            digitalWrite(__pin_latch_mosi, LOW);
            __shiftOut(__pin_data,  __pin_clock, MSBFIRST, test_value);
            __shiftOut(__pin_data,  __pin_clock, MSBFIRST, test_value);
            digitalWrite(__pin_latch_mosi, HIGH);
            delay(1000);
        }
    }
}



void Hc595_Digital_number::__shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
    uint8_t i;
    for(i = 0; i < 8; i++) {
        if(bitOrder == LSBFIRST){
            digitalWrite(dataPin, !!(val & (1 << i)));
        }else{
            digitalWrite(dataPin, !!(val & (1 << (7 - i))));
        }
        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
    }
}




