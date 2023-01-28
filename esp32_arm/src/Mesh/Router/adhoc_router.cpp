#include "all_applications.h"

#ifdef USING_MESH

#include "adhoc_router.h"
#include "MyLibs/basic/logger.h"

// MAC Address of responder - edit as required


void AdhocRouter::__send_out(Package* package){
    // Send message via ESP-NOW
    uint8_t broadcastAddress[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &package, sizeof(package));
   
}


void AdhocRouter::SpinOnce(){
    if (__my_hop == 0xff){
        __time_count_up++;
        if (__time_count_up >= 99999){
            __time_count_up = 0;
            __Broadcast_Iam_Orphan();
            Logger::Info("AdhocRouter::SpinOnce()  __Broadcast_Iam_Orphan() " );
            return;
        }
    }
    // try to find shorter path to net_gate
    for (int i=0; i<ROUTER_TABLE_ROWS; i++){
        Neibour* his = &__my_neibours[i];
        if (his->hop < __my_hop){
            __my_hop = his->hop + 1;
        }
    }
}

void AdhocRouter::Init(){
    __my_hop = 0xff;
    // Init route table
    for (int i=0; i<ROUTER_TABLE_ROWS; i++){
        Neibour* his = &__my_neibours[i];
        his->id = 0;
        his->hop = 0xff;
    }
      // Register callback function
    // esp_now_register_recv_cb(__onReceived);
}

void AdhocRouter::__Broadcast_Iam_Orphan(){
    // Tell Others my mac_addr
    Package orphan_package;
    orphan_package.my_hop = __my_hop;
    // orphan_package.source_net_id = 0;
    // orphan_package.payload = "I am orphan";
    // this->__send_out(&orphan_package);
    const uint8_t* pack = (const uint8_t*) &orphan_package;
    uint8_t len = sizeof(Package);
    this->broadcast(pack, len);
}

// void AdhocRouter::__GetMyMacAddr(uint8_t* mac){
//     esp_read_mac(mac, ESP_MAC_WIFI_STA);

// }

void AdhocRouter::onReceived(const uint8_t * mac, const uint8_t *incomingData, int len){
    for(int i=0; i<ROUTER_TABLE_ROWS; i++){
        Neibour * his = &__my_neibours[i];
        if (his->id == 0){
            // append this node to my_neibours
            his->id = i;
            his->mac_addr[0] = 0;
            his->hop = 0xff;
        }
    }
}

void AdhocRouter::broadcast(const uint8_t* message, uint8_t length){
    // Emulates a broadcast
    // Broadcast a message to every device in range
    uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    esp_now_peer_info_t peerInfo = {};
    memcpy(&peerInfo.peer_addr, broadcastAddress, 6);
    if (!esp_now_is_peer_exist(broadcastAddress)){
        esp_now_add_peer(&peerInfo);
    }
    // Send message
    esp_err_t result = esp_now_send(broadcastAddress,message, length);

    // Print results to serial monitor
    if (result == ESP_OK){
        Serial.println("Broadcast message success");
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

#endif