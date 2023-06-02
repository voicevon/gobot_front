#include "bolt_nut.h"
#include "MyLibs/utility/logger.h"

// void AdhocPackage::PrintOut(const char * title, Neibour* leader){
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

    Serial.print("\tsender(node_id, hop)= (");
    Serial.print(sender_node_id);
    Serial.print(",");
    Serial.print(sender_hop);
    // Serial.print(",");
    // if (leader == NULL) Serial.print("NULL");
    // else  Serial.print(leader->node_id);
    Serial.print(")\t net layer(soure, dest, payload)= (");
    Serial.print(source_node_id);
    Serial.print(",");
    Serial.print(destination_node_id);
    Serial.print(",");
    Serial.print(app_payload[0]);
    Serial.println(")");
}


void Neibour::PrintOut(const char* title){
    Logger::Debug(title);
    // Serial.print("network_id= ");
    // Serial.print(network_id);
    Serial.print("mac_addr: ");
    Serial.print(mac_addr[0]);   Serial.print(" ");
    Serial.print(mac_addr[1]);   Serial.print(" ");
    Serial.print(mac_addr[2]);   Serial.print(" ");
    Serial.print(mac_addr[3]);   Serial.print(" ");
    Serial.print(mac_addr[4]);   Serial.print(" ");
    Serial.print(mac_addr[5]);   Serial.print(" ");

    Serial.print("\tapp_node_id= ");
    Serial.print(node_id);
    Serial.print("\thop= ");
    Serial.print(hop);
    Serial.print("\tleader_ship= ");
    Serial.print(leader_ship);
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
