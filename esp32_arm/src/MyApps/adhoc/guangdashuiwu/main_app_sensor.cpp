

#include "Mesh/Router/adhoc_router.h"
#include "board/board_sensor.h"

#include "all_applications.h"
#ifdef I_AM_GUANGDA_SHUIWU_SENSOR
// #define MY_APP_NODE_ID 2
#define MY_APP_NODE_ID 4

AdhocRouter router;
GuangDa_ShuiWu_SensorBoard board;



void on_esp_now_received(const uint8_t * mac, const uint8_t *incomingData, int len){
    // Logger::Debug("on_esp_now_received");
    router.onReceived( mac, incomingData, len);
}

void setup(){
    board.Init();
    router.Init(MY_APP_NODE_ID);

    esp_now_register_recv_cb(on_esp_now_received);
    Logger::Info("Adhoc Sensor node ,  setup() is done....");
}

void loop(){
    router.Broadcast_Orphan_count_down();
    if (!router.IsJoined_Mesh()) 
        return;
        
    AdhocPackage apc;
    apc.app_source_node_id = MY_APP_NODE_ID;
    apc.app_payload_size = 1;
    apc.app_payload[0] = board.GetPayload();
    router.Send(&apc);
    delay(1000);
}


#endif