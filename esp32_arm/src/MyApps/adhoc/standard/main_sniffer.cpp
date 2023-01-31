

#include "board.h"
#include "Mesh/adhoc/adhoc_sniffer.h"

#include "all_applications.h"
#ifdef I_AM_ADHOC_SNIFFER

AdhocSniffer sniffer;
Adhoc_RouterBoard board;

void on_esp_now_received(const uint8_t * sender_mac, const uint8_t *incomingData, int len){
    // Logger::Debug("on_esp_now_received");
    sniffer.onReceived( sender_mac, incomingData, len)
    board.GetStateLed()->Invert();
}

void setup(){
    board.Init();
    sniffer.Init(111);

    ESP_ERROR_CHECK(esp_now_register_recv_cb(on_esp_now_received));
    Logger::Info("Sniffer App setup()  is done...");

}

void loop(){
    if (sniffer.is_new_rx_package){
        sniffer.is_new_rx_package = false;
        // Ignore net-gate's orphan.
        // if (sniffer.rx_package.sender_hop != 1){
        sniffer.rx_package.PrintOut("  loop()  sniffer got a new Package ");
        // }
    }
}


#endif