#include "board_gate.h"



void GuangDa_ShuiWu_GateBoard::__InitHardware(){
    // init gpio.
	pinMode(PIN_YUNXING_2, OUTPUT);
	pinMode(PIN_ZIDONG_2, OUTPUT);
	pinMode(PIN_GUZHANG_2, OUTPUT);
	pinMode(PIN_YUNXING_4, OUTPUT);
	pinMode(PIN_ZIDONG_4, OUTPUT);
	pinMode(PIN_GUZHANG_4, OUTPUT);
}

void GuangDa_ShuiWu_GateBoard::Init(){
    Serial.begin(115200);
    Serial.print("\n\n\n\n\n\n");
    Logger::Info("==========================================");
    Logger::Info("Hi Xuming, I am Mesh-Adhoc Gate. Good luck......");
    Logger::Info("==========================================");
    Serial.print(COLOR_RESET);
    this->__InitHardware();

    this->RepportRamUsage();
}

