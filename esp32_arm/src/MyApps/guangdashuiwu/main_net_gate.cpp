#include "all_applications.h"
#ifdef I_AM_GUANGDA_SHUIWU_NETGATE

#include "Mesh/Router/adhoc_router.h"
#include "board_gate.h"

GuangDa_ShuiWu_GateBoard board;
AdhocRouter router;


void on_esp_now_received(const uint8_t * mac, const uint8_t *incomingData, int len){
    // Logger::Debug("on_esp_now_received");
    router.onReceived( mac, incomingData, len);
}


void setup(){
    board.Init();
    router.Init(true);

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
    //     // case 4:
    //     digitalWrite(PIN_YUNXING_4, !rx_buffer.io_21);
    //     digitalWrite(PIN_TINGZHI_4, !rx_buffer.io_22);
    //     digitalWrite(PIN_GUZHANG_4, !rx_buffer.io_23);
    // //     break;
    // // case 2:
    //     digitalWrite(PIN_YUNXING_2, !rx_buffer.io_41);
    //     digitalWrite(PIN_TINGZHI_2, !rx_buffer.io_42);
    //     digitalWrite(PIN_GUZHANG_2, !rx_buffer.io_43);
}


#endif