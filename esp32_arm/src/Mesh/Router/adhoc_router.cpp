#include "all_applications.h"

#include "adhoc_router.h"
#include "MyLibs/basic/logger.h"

// void AdhocRouter::SpinOnce(){
//     // Logger::Debug("AdhocRouter::SpinOnce()");
//     _SendOrphan_count_down();   //TODO: send leader_and_me to net_gate
// }


void AdhocRouter::Init(uint8_t my_app_node_id){
    _Init_EspNow();
    esp_read_mac(__my_mac_addr, ESP_MAC_WIFI_STA);
    __my_hop = 255;
    __my_app_node_id = my_app_node_id;
    // __hop_leader_index = -1;

    // init orphan package
    __orphan_package.sender_hop = 255;
    __orphan_package.app_source_node_id = __my_app_node_id;
    __orphan_package.app_payload_size = 1;
    // __orphan_package.payload = " I am orphan";
    // Init route table
    for (int i=0; i<ROUTER_TABLE_ROWS; i++){
        Neibour* his = &__my_neibours[i];
        his->app_node_id = 0;  // Not an avaliable node.
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
        blank->hop = incoming_package->sender_hop;
        blank->qos = 90;
        AdhocHelper::CopyMacAddr(the_mac, blank->mac_addr);
        if (__my_leader == NULL){
            if (blank->hop < __my_hop){
                // set as a temperory leader.
                __my_leader = blank;
                __my_hop = __my_leader->hop + 1;
                Logger::Info("AdhocRouter::__sniff_air_package():  Got a temperory leader");
                Logger::Print("__my_hop", __my_hop);
            }
        }
    }else{
        // might update his hop, even my_hop
        his->hop = incoming_package->sender_hop;
        his->qos++;
        if (his->qos > 100){
            // lower the qos level of all neibous.
            for (int i=0 ; i<ROUTER_TABLE_ROWS; i++){
                if (__my_neibours[i].app_node_id > 0){
                    __my_neibours[i].qos--;
                    if (__my_neibours[i].qos == 0 ){
                        // remove this from routing_table.
                        __my_neibours[i].app_node_id = 0;
                    }
                }
            }
            if (his->hop <= __my_hop){
                if (his != __my_leader){
                    // This neibour is my new leader
                    __my_leader = his;
                    Logger::Info("AdhocRouter::__sniff_air_package()  Got a better leader");
                    Logger::Print("__my_hop", __my_hop);
                }
            }
        }
    }
}

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
        if (__my_leader != NULL){   // Is this necessary?  Any way, safe firstly.
            // forward the package ,
            incoming_package->sender_hop = __my_hop; 
            AdhocHelper::CopyMacAddr(__my_leader->mac_addr , incoming_package->to_mac_addr);
            Send(incoming_package);
        }
    }

}
