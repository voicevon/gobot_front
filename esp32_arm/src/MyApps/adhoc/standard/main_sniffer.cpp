#include "all_applications.h"
#ifdef I_AM_ADHOC_SNIFFER

#include "board.h"
#include "Mesh/Router/adhoc_sniffer.h"

AdhocSniffer sniffer;
Adhoc_RouterBoard board;

void on_esp_now_received(const uint8_t * mac, const uint8_t *incomingData, int len){
    // Logger::Debug("on_esp_now_received");
    sniffer.onReceived( mac, incomingData, len);
}

void setup(){
    board.Init();
    sniffer.Init();

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
    sniffer.SpinOnce();
    if (sniffer.is_new_rx_package){
        sniffer.rx_package.PrintOut("sniffer got a new Package ");
    }
}


#endif