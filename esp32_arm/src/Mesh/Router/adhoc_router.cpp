#include "all_applications.h"

#include "adhoc_router.h"
#include "MyLibs/basic/logger.h"



void AdhocRouter::Init(uint8_t my_app_node_id){
    _Init_EspNow();
    esp_read_mac(_my_mac_addr, ESP_MAC_WIFI_STA);
    _my_hop = 100;
    _my_app_node_id = my_app_node_id;

    // init orphan package
    _orphan_package.sender_hop = _my_hop;
    _orphan_package.app_source_node_id = _my_app_node_id;
    _orphan_package.app_payload_size = 1;
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

void AdhocRouter::__append_to_neibours(uint8_t * sender_mac, AdhocPackage* incoming_package){
        Neibour* blank = __find_blank_neibour();
        if (blank == NULL){
            // no more space , my neibouts count is more than MAX_TABLE_ROWS == 16
            return;
        }
        // just save to neibour_table,  we don't set hop right now.
        // Logger::Print("sender's  app_node_id", incoming_package-;

        blank->app_node_id = incoming_package->app_source_node_id;
        blank->hop = incoming_package->sender_hop;
        blank->qos = 90;
        AdhocHelper::CopyMacAddr(sender_mac, blank->mac_addr);
        Logger::Info("AdhocRouter::__sniff_air_package() -> __append_to_neibours()");
        blank->PrintOut("new neibour detail");

        if (__my_leader == NULL){
            if (blank->hop < _my_hop){
                // set as a temperory leader.
                __my_leader = blank;
                _my_hop = __my_leader->hop + 1;
                Logger::Info("AdhocRouter::__sniff_air_package() -> __append_to_neibours():  Got a temperory leader");
                __my_leader->PrintOut("__my_leader detail");
                Logger::Print("__my_hop", _my_hop);
            }
        }
}

void AdhocRouter::__try_to_remove_bad_neibour(){
    // lower the qos level of all neibous.
    for (int i=0 ; i<ROUTER_TABLE_ROWS; i++){
        if (__my_neibours[i].app_node_id > 0){
            __my_neibours[i].qos--;
            if (__my_neibours[i].qos < 0 ){
                // remove this from routing_table.
                Logger::Info("AdhocRouter::__sniff_air_package()  remove this neibour.");
                __my_neibours[i].PrintOut("his detail");
                __my_neibours[i].app_node_id = 0;
                if (&__my_neibours[i] == __my_leader){
                    Logger::Print("The removed neibout is my_leader, his app_node_id",__my_leader->app_node_id);
                    __my_leader = NULL;
                    _my_hop = 100;
                }
            }
        }
    }
}

void AdhocRouter::__sniff_air_package(const uint8_t * mac, AdhocPackage* incoming_package){
    // incoming_package->PrintOut("from:  AdhocRouter::onReceived() ");
    uint8_t*  the_mac = (uint8_t*) (mac);
    Neibour* sender = __search_neibour(the_mac);
    if (sender == NULL){
        // append to routing_table
        __append_to_neibours(the_mac, incoming_package);
    }else{
        // might update sender's hop in routing_table, even my_hop
        sender->hop = incoming_package->sender_hop;
        if (sender == __my_leader){
            // leader's hop might becomes greater
            _my_hop = sender->hop + 1;
        }
        sender->qos += (int)_my_hop - (int)sender->hop;  
        if (sender->qos > 100){
            __try_to_remove_bad_neibour();
            if (sender != __my_leader){
                // This neibour is my new leader
                __my_leader = sender;
                _my_hop = __my_leader->hop + 1;
                Logger::Info("AdhocRouter::__sniff_air_package()  Got a better leader");
                __my_leader->PrintOut("Leader details");
                Logger::Print("__my_hop", _my_hop);
                
            }
        }
    }
}

bool AdhocRouter::onReceived(const uint8_t * sender_mac, const uint8_t *incomingData, int len){
    bool debug = false;
    if (debug){
        Logger::Debug("AdhocRouter::onReceived");
        Logger::Print("len", len);
        Serial.print("to_mac_addr= ");
        for(int i=0; i<6; i++){
            Serial.print(" ");
            Serial.print(*(sender_mac+i));
        }
        Serial.println("");
    }

    // from incomingDate to package, might effect routing_table
    AdhocPackage* incoming_package = (AdhocPackage*) (incomingData);
    __sniff_air_package(sender_mac, incoming_package);

    if (AdhocHelper::IsSameMacAddr(incoming_package->to_mac_addr, _my_mac_addr)){
        // I am the target node of the package.
        if (__my_leader != NULL){   // Is this necessary?  Any way, safe firstly.
            // forward the package ,
            incoming_package->PrintOut("AdhocRouter::onReceived()   forwarding this...");
            Send_App_Package(incoming_package);
        }
    }

    if (AdhocHelper::IsSameMacAddr(__my_leader->mac_addr, (uint8_t* )(sender_mac))){
        return true;
    }
    return false;

}


void AdhocRouter::Send_App_Package(AdhocPackage* app_pkg){
   app_pkg->sender_hop = _my_hop;
   AdhocHelper::CopyMacAddr(__my_leader->mac_addr, app_pkg->to_mac_addr);
   _Send(app_pkg);
}
