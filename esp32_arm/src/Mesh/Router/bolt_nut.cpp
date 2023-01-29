#include "bolt_nut.h"
#include "MyLibs/basic/logger.h"

void AdhocPackage::PrintOut(const char * title){
    Logger::Debug(title);
    // Serial.print("network_id= ");
    // Serial.print(network_id);
    Serial.print("to mac_addr: ");
    Serial.print(to_mac_addr[0]);   Serial.print(" ");
    Serial.print(to_mac_addr[1]);   Serial.print(" ");
    Serial.print(to_mac_addr[2]);   Serial.print(" ");
    Serial.print(to_mac_addr[3]);   Serial.print(" ");
    Serial.print(to_mac_addr[4]);   Serial.print(" ");
    Serial.print(to_mac_addr[5]);   Serial.print(" ");

    Serial.print("\tsender_hop= ");
    Serial.print(sender_hop);
    Serial.print("\tsource_node= ");
    Serial.print(app_source_node_id);
    Serial.print("    payload[0]=");
    Serial.print(app_payload[0]);
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
