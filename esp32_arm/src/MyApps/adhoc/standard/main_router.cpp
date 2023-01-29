#include "all_applications.h"
#ifdef I_AM_ADHOC_ROUTER

#include "board_router.h"
#include "Mesh/Router/adhoc_router.h"

AdhocRouter router;
Adhoc_RouterBoard board;

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
}


#endif