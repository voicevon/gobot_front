#include "board.h"
#include "Mesh/Router/adhoc_router.h"


#include "all_applications.h"
#ifdef I_AM_ADHOC_ROUTER
#define MY_APP_NODE_ID 102
AdhocRouter router;
Adhoc_RouterBoard board;

void on_esp_now_received(const uint8_t * mac, const uint8_t *incomingData, int len){
    // Logger::Debug("on_esp_now_received");
    router.onReceived( mac, incomingData, len);
}

void setup(){
    board.Init();
    router.Init(MY_APP_NODE_ID);

    esp_now_register_recv_cb(on_esp_now_received);

    Logger::Info("Adhoc Router App setup()  is done...");

}

void loop(){
    router.Broadcast_Orphan_count_down();
}


#endif