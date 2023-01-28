#include "all_applications.h"
#ifdef I_AM_GUANGDA_SHUIWU_SENSOR

#include "Mesh/Router/adhoc_router.h"
#include "board_sensor.h"



AdhocRouter router;
GuangDa_ShuiWu_SensorBoard board;



void on_esp_now_received(const uint8_t * mac, const uint8_t *incomingData, int len){
    // Logger::Debug("on_esp_now_received");
    router.onReceived( mac, incomingData, len);
}

void setup(){
    board.Init();
    router.Init(false);
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
    Package pc;
    pc.payload[0] = board.GetPayload();
    router.Send(&pc);
    delay(1000);
}


#endif