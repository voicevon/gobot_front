#include "all_applications.h"

#ifdef USING_MESH

#include "adhoc_router.h"
#include "WiFi.h"



void AdhocRouter::SpinOnce(){
    if (__my_hop == 0xff){
        __Broadcast_Iam_Orphan();
        return;
    }
    // try to find shorter path to net_gate
    for (int i=0; i<ROUTER_TABLE_ROWS; i++){
        if (__my_neibours[i].hop < __my_hop){
            __my_hop = __my_neibours[i].hop + 1;
        }
    }
}

void AdhocRouter::Init(){
    // Init route table
    for (int i=0; i<ROUTER_TABLE_ROWS; i++){
        __my_neibours[i].id = 0;
    }
}

void AdhocRouter::__Broadcast_Iam_Orphan(){
    // Tell Others my mac_addr

}

void AdhocRouter::__GetMyMacAddr(uint8_t* mac){
    esp_read_mac(mac, ESP_MAC_WIFI_STA);

}

void AdhocRouter::__onReceived_Orphan(){
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