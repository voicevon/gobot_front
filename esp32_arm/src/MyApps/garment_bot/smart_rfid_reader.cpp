#include "smart_rfid_reader.h"

void SmartRfidReader::Init(int8_t pin_clk, int8_t pin_miso, int8_t pin_mosi){
	this->__rfidReader = new MFRC522();
}

bool SmartRfidReader::ReadCard(){
    bool ok = this->__rfidReader->PICC_ReadCardSerial();
    if (ok){
        this->CardId = 1;
        this->MainRoad_IsOn_LeftSide = true;
        this->NextStationType = 3;
    }
    return ok;
    // if(ok){
    //     this->__onDetectCard(this->CardId);
    // }
}
