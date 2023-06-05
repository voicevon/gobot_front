#include "board.h"



void Adhoc_RouterBoard::__InitHardware(){
    // init gpio.
    // pinMode(PIN_STATE_LED, OUTPUT);
    __rx_led.Init(0, PIN_STATE_LED, HIGH);
}

void Adhoc_RouterBoard::Init(const char* app_welcome_statement){
    Serial.begin(115200);
    Serial.print("\n\n\n\n\n\n");
    Logger::Info("==========================================");
    Logger::Info("Hi Xuming, I am Mesh-Adhoc Router or Sniffer. Good luck......");
    Logger::Info("==========================================");
    Serial.print(COLOR_RESET);
    this->__InitHardware();

    this->RepportRamUsage();
}

