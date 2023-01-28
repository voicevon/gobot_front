#include "all_applications.h"

#include "adhoc_router.h"
#include "MyLibs/basic/logger.h"


void AdhocRouter::SpinOnce(){
    // Logger::Debug("AdhocRouter::SpinOnce()");
    // Logger::Print("'__my_hop", __my_hop);
    if ((__my_hop == 1) || (__my_hop == 0xff)){
        __time_count_up++;
        if (__time_count_up >= 3999999){
            __time_count_up = 0;
            Logger::Info("AdhocRouter::SpinOnce()  Broadcasting: I_am_Orphan() " );
            Logger::Print("__my_hop", __my_hop);
            // __Broadcast_Iam_Orphan();
            Send(&__orphan_package);
            return;
        }
    }
    // try to find best header.
    __forward_to = -1;
}

bool AdhocRouter::IsJoined_Mesh(){
    if (__my_hop < 200) return true;
    return false;
}

void AdhocRouter::Init(){
    esp_read_mac(__my_mac_addr, ESP_MAC_WIFI_STA);
    __my_hop = 0xff;
    __forward_to = -1;

    // init orphan package
    __orphan_package.my_hop = __my_hop;
    __orphan_package.app_payload_size = 1;
    // __orphan_package.payload = " I am orphan";
    // Init route table
    for (int i=0; i<ROUTER_TABLE_ROWS; i++){
        Neibour* his = &__my_neibours[i];
        his->id = 0;
        his->hop = 0xff;
    }
}

Neibour* AdhocRouter::__search_neibour(uint8_t * mac_addr){
    for(int i=0; i<ROUTER_TABLE_ROWS; i++){
        if (AdhocHelper::IsSameMacAddr(mac_addr, __my_neibours[i].mac_addr)){
            return &__my_neibours[i];
        }
    }
    return NULL;
}

Neibour* AdhocRouter::__find_blank_neibour(){
    for(int i=0; i<ROUTER_TABLE_ROWS; i++){
        if (__my_neibours[i].id == 0){
            return &__my_neibours[i];
        }
    }
    return NULL;
}

void AdhocRouter::__sniff_air_package(const uint8_t * mac, AdhocPackage* incoming_package){
    // incoming_package->PrintOut("from:  AdhocRouter::onReceived() ");
    uint8_t*  the_mac = (uint8_t*) (mac);
    Neibour* his = __search_neibour(the_mac);
    if (his == NULL){
        // append to routing_table
        Neibour* blank = __find_blank_neibour();
        if (blank == NULL){
            // no more space 
            return;
        }
        // just save to neibour_table,  we don't set hop right now.
        blank->id = 1;
        AdhocHelper::CopyMacAddr(the_mac, blank->mac_addr);
        blank->hop = 0xff;
    }else{
        // might update his hop, even my_hop
        his->hop = incoming_package->my_hop;
        if (his->hop + 1 < __my_hop){
            __my_hop = his->hop + 1;
            Logger::Info("AdhocRouter::onReceived()  got shorter path to net_gate");
            Logger::Print("incoming_package hop", incoming_package->my_hop);
            Logger::Print("his hop", his->hop);
            Logger::Print("now my_hop", __my_hop);
        }
    }
}

// TODO:  the shortest routing might be loss connection.
void AdhocRouter::onReceived(const uint8_t * mac, const uint8_t *incomingData, int len){
    bool debug = false;
    if (debug){
        Logger::Debug("AdhocRouter::onReceived");
        Logger::Print("len", len);
        Serial.print("mac_addr= ");
        for(int i=0; i<6; i++){
            Serial.print(" ");
            Serial.print(*(mac+i));
        }
        Serial.println("");
    }

    // from incomingDate to package, might effect routing_table
    AdhocPackage* incoming_package = (AdhocPackage*) (incomingData);
    __sniff_air_package(mac, incoming_package);

    if (AdhocHelper::IsSameMacAddr(incoming_package->to_mac_addr, __my_mac_addr)){
        // I am the target node of the package.
        if (__my_hop < 200){
            // have joint a network,  forward the package , 
            AdhocHelper::CopyMacAddr(__my_neibours[__forward_to].mac_addr , incoming_package->to_mac_addr);
            Send(incoming_package);
        }
    }

}
