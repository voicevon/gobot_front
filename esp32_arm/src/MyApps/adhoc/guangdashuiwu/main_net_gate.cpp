#include "all_applications.h"
#ifdef I_AM_GUANGDA_SHUIWU_NETGATE

#include "Mesh/Router/adhoc_gate.h"
#include "board/board_gate.h"

GuangDa_ShuiWu_GateBoard board;
AdhocGate gate;


void on_esp_now_received(const uint8_t * mac, const uint8_t *incomingData, int len){
    // Logger::Debug("on_esp_now_received");
    gate.onReceived( mac, incomingData, len);
}


void setup(){
    board.Init();
    gate.Init();

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
    gate.SpinOnce();
    if (gate.is_new_rx_package){
        gate.is_new_rx_package = false;
        gate.rx_package.PrintOut("loop()  new rx_package");
    }
    uint8_t io = gate.rx_package.app_payload[0];
    if (gate.rx_package.app_source_node_id == 4){
        digitalWrite(PIN_YUNXING_4, ! (io & 1));
        digitalWrite(PIN_ZIDONG_4, !(io & 2));
        digitalWrite(PIN_GUZHANG_4, !(io & 4));
    }
    if (gate.rx_package.app_source_node_id == 2){
        digitalWrite(PIN_YUNXING_2, !(io & 1));
        digitalWrite(PIN_ZIDONG_2, !(io & 2));
        digitalWrite(PIN_GUZHANG_2, !(io & 4));
    }
}


#endif