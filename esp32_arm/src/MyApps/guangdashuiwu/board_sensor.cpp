#include "board_sensor.h"



void GuangDa_ShuiWu_SensorBoard::__InitHardware(){
    // init gpio.


    pinMode(PIN_YUNXING, INPUT_PULLUP);
    pinMode(PIN_TINGZHI, INPUT_PULLUP);
    pinMode(PIN_GUZHANG, INPUT_PULLUP);
}

void GuangDa_ShuiWu_SensorBoard::Init(){
    Serial.begin(115200);
    Serial.print("\n\n\n\n\n\n");
    Logger::Info("==========================================");
    Logger::Info("Hi Xuming, I am Mesh-Adhoc Sensor. Good luck......");
    Logger::Info("==========================================");
    Serial.print(FCBC_RESET);
    this->__InitHardware();

    this->RepportRamUsage();
}

