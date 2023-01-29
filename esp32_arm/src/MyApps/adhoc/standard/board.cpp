#include "board.h"



void GuangDa_ShuiWu_RouterBoard::__InitHardware(){
    // init gpio.
    pinMode(PIN_STATE_LED, OUTPUT);
}

void GuangDa_ShuiWu_RouterBoard::Init(){
    Serial.begin(115200);
    Serial.print("\n\n\n\n\n\n");
    Logger::Info("==========================================");
    Logger::Info("Hi Xuming, I am Mesh-Adhoc Router. Good luck......");
    Logger::Info("==========================================");
    Serial.print(FCBC_RESET);
    this->__InitHardware();

    this->RepportRamUsage();
}

