#include "all_applications.h"

#ifdef USING_MESH

#include "adhoc_router.h"
#include "WiFi.h"
#include <esp_now.h>

// MAC Address of responder - edit as required
uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0x7A, 0xAE, 0x7C};


void AdhocRouter::__send_out(Package* package){
      // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &package, sizeof(package));
   
}


void AdhocRouter::SpinOnce(){
    if (__my_hop == 0xff){
        __time_count_up++;
        if (__time_count_up >= 99999){
            __time_count_up = 0;
            __Broadcast_Iam_Orphan();
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
    // Init route table
    for (int i=0; i<ROUTER_TABLE_ROWS; i++){
        Neibour* his = &__my_neibours[i];
        his->id = 0;
        his->hop = 0xff;
    }
      // Register callback function
    esp_now_register_recv_cb(__onReceived);
}

void AdhocRouter::__Broadcast_Iam_Orphan(){
    // Tell Others my mac_addr
    Package orphan_package;
    orphan_package.my_hop = __my_hop;
    // orphan_package.source_net_id = 0;
    // orphan_package.payload = "I am orphan";
    this->__send_out(&orphan_package);
}

// void AdhocRouter::__GetMyMacAddr(uint8_t* mac){
//     esp_read_mac(mac, ESP_MAC_WIFI_STA);

// }

void AdhocRouter::__onReceived(const uint8_t * mac, const uint8_t *incomingData, int len){
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


#endif