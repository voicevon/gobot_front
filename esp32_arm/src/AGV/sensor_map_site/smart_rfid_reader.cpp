#include "smart_rfid_reader.h"

void SmartRfidReader::Init(int8_t pin_clk, int8_t pin_miso, int8_t pin_mosi){
	this->__rfidReader = new MFRC522();
}

int SmartRfidReader::ReadMapSiteId(){
    return 1234;
}

bool SmartRfidReader::ReadCard(){
    bool ok = this->__rfidReader->PICC_ReadCardSerial();
    if (ok){
        this->CardId = 1;
        this->MainRoad_IsOn_LeftSide = true;
        this->NextStationType = WorkStation::TYPE_ON_MAP::LOAD_UNLOAD;
    }
    return ok;
    // if(ok){
    //     this->__onDetectCard(this->CardId);
    // }
}


void SmartRfidReader::SayHello(){
    
}