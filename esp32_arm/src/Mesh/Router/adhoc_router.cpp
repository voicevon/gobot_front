#include "all_applications.h"

#include "adhoc_router.h"
#include "MyLibs/basic/logger.h"

Neibour* AdhocRouter::__find_best_leader(){
    // So, What is the best,  lowest hop and stalability. consider these two together.
    Neibour* leader = NULL;
    Neibour* index;
    int min_hop = 200;
    for (int i=0; i<ROUTER_TABLE_ROWS; i++){
        index = &__my_neibours[i];
        if (index->hop < min_hop){
            leader = index;
            min_hop = index->hop;
        }
    }
    return leader;
}

void AdhocRouter::SpinOnce(){
    // Logger::Debug("AdhocRouter::SpinOnce()");
    static Neibour* last_leader = NULL;
    _SendOrphan_count_down();
    // try to find best header from routing_table.
    Neibour* new_leader = __find_best_leader();
    if (new_leader != last_leader){
        Logger::Info("AdhocRouter::SpinOnce()  Got New leader");
        Logger::Print("__my_hop", __my_hop);
        __my_hop = new_leader->hop + 1;
        last_leader = new_leader;
    }
}

bool AdhocRouter::IsJoined_Mesh(){
    if (__my_hop < 200) return true;
    return false;
}

void AdhocRouter::Init(uint8_t my_app_node_id){
    _Init_EspNow();
    esp_read_mac(__my_mac_addr, ESP_MAC_WIFI_STA);
    __my_hop = 255;
    __my_app_node_id = my_app_node_id;
    __hop_leader_index = -1;

    // init orphan package
    __orphan_package.sender_hop = 255;
    __orphan_package.app_source_node_id = __my_app_node_id;
    __orphan_package.app_payload_size = 1;
    // __orphan_package.payload = " I am orphan";
    // Init route table
    for (int i=0; i<ROUTER_TABLE_ROWS; i++){
        Neibour* his = &__my_neibours[i];
        his->app_node_id = 0;
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
        if (__my_neibours[i].app_node_id == 0){
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
            // no more space , my neibouts count is more than MAX_TABLE_ROWS == 16
            return;
        }
        // just save to neibour_table,  we don't set hop right now.
        blank->app_node_id = incoming_package->app_source_node_id;
        AdhocHelper::CopyMacAddr(the_mac, blank->mac_addr);
        blank->hop = 0xff;
    }else{
        // might update his hop, even my_hop
        his->hop = incoming_package->sender_hop;
    }
}

// TODO:  the shortest routing might be loss connection.
void AdhocRouter::onReceived(const uint8_t * mac, const uint8_t *incomingData, int len){
    bool debug = false;
    if (debug){
        Logger::Debug("AdhocRouter::onReceived");
        Logger::Print("len", len);
        Serial.print("to_mac_addr= ");
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
            AdhocHelper::CopyMacAddr(__my_neibours[__hop_leader_index].mac_addr , incoming_package->to_mac_addr);
            Send(incoming_package);
        }
    }

}
