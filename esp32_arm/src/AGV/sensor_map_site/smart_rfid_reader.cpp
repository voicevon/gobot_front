#include "smart_rfid_reader.h"

// void SmartRfidReader::Init(int8_t pin_clk, int8_t pin_miso, int8_t pin_mosi){
// 	this->__rfidReader = new MFRC522();
// }

int SmartRfidReader::ReadMapSiteId(){
    for (byte i = 0; i < this->__mfrc522->uid.size; i++) {
        Serial.print(this->__mfrc522->uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(this->__mfrc522->uid.uidByte[i], HEX);
    }
    return 1234;
}

bool SmartRfidReader::ReadCard(){
    bool ok = this->__mfrc522->PICC_ReadCardSerial();
    if (ok){
        this->CardId = 1;
        this->MainRoad_IsOn_LeftSide = true;
        this->NextStationType = WorkStation::TYPE_ON_MAP::LOAD_UNLOAD;
    }
    return ok;
}


