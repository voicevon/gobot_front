#include "board.h"


#define PIN_STATE_LED 2

void WaterDropper_Board::__InitHardware(){
    __state_led.Init(0, PIN_STATE_LED, HIGH);

}

void WaterDropper_Board::Init(const char* app_welcome_statement){
    _InitSerial("Hi Xuming, I am WaterDropper board. Good luck......");
    Logger::Info(app_welcome_statement);
    Serial.print(COLOR_RESET);
    this->__InitHardware();
    this->_Init_SPIFFS();
    this->RepportRamUsage();
}

