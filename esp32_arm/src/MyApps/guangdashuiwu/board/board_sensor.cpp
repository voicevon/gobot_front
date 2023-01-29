#include "board_sensor.h"


uint8_t GuangDa_ShuiWu_SensorBoard::GetPayload(){
    uint8_t payload = 0;
    if (__zidong_shoudong.IsPressed()) payload += 1;
    if (__yunxing_tingzhi.IsPressed()) payload += 2;
    if (__guzhang_zhengchang.IsPressed()) payload +=4;
}


void GuangDa_ShuiWu_SensorBoard::Init(){
    Serial.begin(115200);
    Serial.print("\n\n\n\n\n\n");
    Logger::Info("==========================================");
    Logger::Info("Hi Xuming, I am Mesh-Adhoc Sensor. Good luck......");
    Logger::Info("==========================================");
    Serial.print(FCBC_RESET);
    this->RepportRamUsage();
}

