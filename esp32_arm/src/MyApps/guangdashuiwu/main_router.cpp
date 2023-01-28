#include "all_applications.h"
#ifdef I_AM_GUANGDA_SHUIWU_ROUTER

#include "board_router.h"
#include "Mesh/Router/adhoc_router.h"

AdhocRouter router;
GuangDa_ShuiWu_RouterBoard board;

void on_esp_now_received(const uint8_t * mac, const uint8_t *incomingData, int len){
    router.onReceived( mac, incomingData, len);
}

void setup(){
    board.Init();

    WiFi.mode(WIFI_STA);
    // Initilize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_recv_cb(on_esp_now_received);

}

void loop(){
    // router.SpinOnce();
    // AdhocRouter::Instance().SpinOnce();
}


#endif