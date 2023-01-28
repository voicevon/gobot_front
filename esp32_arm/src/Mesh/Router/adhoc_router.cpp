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

}

void AdhocRouter::Init(bool i_am_net_gate){
    __my_hop = 0xff;
    if (i_am_net_gate) __my_hop = 1;

    // init orphan package
    __orphan_package.my_hop = __my_hop;
    __orphan_package.package_size = 1;
    // __orphan_package.payload = " I am orphan";
    // Init route table
    for (int i=0; i<ROUTER_TABLE_ROWS; i++){
        Neibour* his = &__my_neibours[i];
        his->id = 0;
        his->hop = 0xff;
    }
      // Register callback function
    // esp_now_register_recv_cb(__onReceived);
}

// void AdhocRouter::__Broadcast_Iam_Orphan(){
//     // Logger::Debug("AdhocRouter::__Broadcast_Iam_Orphan()");
//     // Tell Others my mac_addr
//     // AdhocPackage orphan_package;
//     // orphan_package.my_hop = __my_hop;
//     // orphan_package.source_net_id = 0;
//     // orphan_package.payload = "I am orphan";
//     // this->__send_out(&orphan_package);
//     const uint8_t* pack = (const uint8_t*) &__orphan_package;
//     uint8_t len = sizeof(AdhocPackage);
//     // Logger::Print("len", len);
//     this->broadcast(pack, len);
// }

bool AdhocRouter::__is_same_mac_addr(uint8_t* addr_a, uint8_t* addr_b){
    for (int i=0; i<6; i++){
        if (*(addr_a+i) != *(addr_b+i))
            return false;
    }
    return true;
}

Neibour* AdhocRouter::__search_neibour(uint8_t * mac_addr){
    for(int i=0; i<ROUTER_TABLE_ROWS; i++){
        if (__is_same_mac_addr(mac_addr, __my_neibours[i].mac_addr)){
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

void AdhocRouter::__onPackage_received(const uint8_t * mac, AdhocPackage* incoming_package){
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
        blank->mac_addr[0] = mac[0];
        blank->mac_addr[1] = mac[1];
        blank->mac_addr[2] = mac[2];
        blank->mac_addr[3] = mac[3];
        blank->mac_addr[4] = mac[4];
        blank->mac_addr[5] = mac[5];
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
    __onPackage_received(mac, incoming_package);

    // if (incoming_package->payload == 'This is an orphan package'){
    //     return;

    // if (this_is_a_repeated_message){
    //     // to avoid message flooding.
    //     return;
    // }
    Send(incoming_package);
}

// void AdhocRouter::broadcast(const uint8_t* message, uint8_t length){
void AdhocRouter::Send(AdhocPackage* pkg){
    // Emulates a broadcast
    // Broadcast a message to every device in range
    uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    esp_now_peer_info_t peerInfo = {};
    memcpy(&peerInfo.peer_addr, broadcastAddress, 6);
    if (!esp_now_is_peer_exist(broadcastAddress)){
        esp_now_add_peer(&peerInfo);
    }
    // Send message
    const uint8_t* message = (const uint8_t*) pkg;
    uint8_t length = sizeof(AdhocPackage);
    esp_err_t result = esp_now_send(broadcastAddress, message, length);

    // Print results to serial monitor
    if (result == ESP_OK){
        Serial.print("Tx ");
    } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
        Serial.println("ESP-NOW not Init.");
    } else if (result == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Invalid Argument");
    } else if (result == ESP_ERR_ESPNOW_INTERNAL)  {
        Serial.println("Internal Error");
    } else if (result == ESP_ERR_ESPNOW_NO_MEM)  {
        Serial.println("ESP_ERR_ESPNOW_NO_MEM");
    } else if (result == ESP_ERR_ESPNOW_NOT_FOUND)  {
        Serial.println("Peer not found.");
    } else {
        Serial.println("Unknown error");
    }
}
