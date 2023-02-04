

#include "Mesh/adhoc/adhoc_router.h"
#include "board/board_sensor.h"

#include "all_applications.h"
#ifdef I_AM_GUANGDA_SHUIWU_SENSOR
#define MY_APP_NODE_ID 2
// #define MY_APP_NODE_ID 4

AdhocRouter router;
GuangDa_ShuiWu_SensorBoard board;

void on_esp_now_received(const uint8_t * sender_mac, const uint8_t *incomingData, int len){
    // Logger::Debug("on_esp_now_received");
    if (router.onReceived( sender_mac, incomingData, len)){
        board.GetStateLed()->Invert();
    }
}

void setup(){
    board.Init();
    router.Init(MY_APP_NODE_ID);

    esp_now_register_recv_cb(on_esp_now_received);
    Logger::Info("Adhoc Sensor node ,  setup() is done....");
}

void loop(){
    board.SpinOnce();
    router.Broadcast_Orphan_count_down();
    if (router.IamOrphan()) 
        return;
        
    AdhocPackage apc;
    apc.source_node_id = MY_APP_NODE_ID;
    apc.destination_node_id = NODE_ID_MESH_GATE;  
    apc.app_payload_size = 1;
    apc.app_payload[0] = board.GetPayload();
    router.Send_App_Package(&apc);
    // apc.PrintOut("loop()  is sent.");
    delay(1000);
}


#endif