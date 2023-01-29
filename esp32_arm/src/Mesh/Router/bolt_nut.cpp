#include "bolt_nut.h"
#include "MyLibs/basic/logger.h"

void AdhocPackage::PrintOut(const char * title){
    Logger::Info(title);
    // Serial.print("network_id= ");
    // Serial.print(network_id);
    Serial.print("\thop= ");
    Serial.print(my_hop);
    Serial.print("\tsource_node= ");
    Serial.print(app_source_node_id);
    Serial.print("\tpayload[0]=");
    Serial.print(app_payload[0]);
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
