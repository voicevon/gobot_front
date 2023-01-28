#include "all_applications.h"
#ifdef I_AM_GUANGDA_SHUIWU_SENSOR

#include "Mesh/Router/adhoc_router.h"
#include "board_sensor.h"

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
    router.Init();
    WiFi.mode(WIFI_STA);
    // Initilize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Logger::Error("Error initializing ESP-NOW");
        Logger::Halt("????");
        return;
    }
    esp_now_register_recv_cb(on_esp_now_received);
}

void loop(){
    router.SpinOnce();
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