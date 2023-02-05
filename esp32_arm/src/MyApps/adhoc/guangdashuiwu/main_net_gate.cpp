
#include "Mesh/adhoc/adhoc_gate.h"
#include "board/board_gate.h"

#include "all_applications.h"
#ifdef I_AM_GUANGDA_SHUIWU_NETGATE

GuangDa_ShuiWu_GateBoard board;
AdhocGate gate;
#define MY_APP_NODE_ID NODE_ID_MESH_GATE

void on_esp_now_received(const uint8_t * sender_mac, const uint8_t *incomingData, int len){
    // Logger::Debug("on_esp_now_received");
    gate.onReceived(sender_mac, incomingData, len);
}


void setup(){
    board.Init();
    gate.Init(MY_APP_NODE_ID);
    esp_now_register_recv_cb(on_esp_now_received);
    Logger::Info("Guangda Shuiwu net_gate    setup() is done.....;.");
}

void loop(){
    gate.Broadcast_Orphan_count_down();
    if (gate.received_new_app_package){
        gate.received_new_app_package = false;
        uint8_t io = gate.rx_package.app_payload[0];
        // gate.rx_package.PrintOut("loop()  new rx_package");
        Serial.print(gate.rx_package.source_node_id);
        Serial.print("------");
        Serial.println(io);
        if (gate.rx_package.source_node_id == 4){
            digitalWrite(PIN_ZIDONG_4,  (io & 1));
            digitalWrite(PIN_YUNXING_4, (io & 2));     
            digitalWrite(PIN_GUZHANG_4, (io & 4));
        }
        if (gate.rx_package.source_node_id == 2){
            digitalWrite(PIN_ZIDONG_2,  (io & 1));
            digitalWrite(PIN_YUNXING_2, (io & 2));
            digitalWrite(PIN_GUZHANG_2, (io & 4));
        }
    }
}


#endif
