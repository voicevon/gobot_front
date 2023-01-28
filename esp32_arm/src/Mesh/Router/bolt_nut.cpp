#include "bolt_nut.h"
#include "MyLibs/basic/logger.h"

void AdhocPackage::PrintOut(const char * title){
    Logger::Info(title);
    Serial.print("source=");
    Serial.print(network_id);
    Serial.print(" hop= ");
    Serial.print(my_hop);
    Serial.println(" ");
}


bool AdhocHelper::IsSameMacAddr(uint8_t* addr_a, uint8_t* addr_b){
    for (int i=0; i<6; i++){
        if (*(addr_a+i) != *(addr_b+i))
            return false;
    }
    return true; 
}

void AdhocHelper::CopyMacAddr(uint8_t* source, uint8_t* target){
    for (int i=0; i<6; i++){
        *(target + i) = *(source + i);
    }
}
