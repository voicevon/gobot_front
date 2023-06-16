#include "adhoc_gate.h"
#include "von/cpp/utility/logger.h"

bool AdhocGate::onReceived(const uint8_t* sender_mac, const uint8_t* incomingData, int len) {
    memcpy(&rx_package ,incomingData, len);
    if (AdhocHelper::IsSameMacAddr(rx_package.to_mac_addr, _my_mac_addr)){
        // Logger::Info("AdhocGate::onReceived() , was  to my addr");
        received_new_app_package = true;
    }
    return false;   // A gate has no leader.
}



void AdhocGate::Init(uint8_t my_node_id){
    _Init_EspNow();
    esp_read_mac(_my_mac_addr, ESP_MAC_WIFI_STA);
    received_new_app_package = false;
    _my_hop = 1;
    _my_node_id = my_node_id;
    _orphan_package.sender_node_id = _my_node_id;
    _orphan_package.sender_hop= 1;
    _orphan_package.app_payload_size = 1;
    _orphan_package.to_mac_addr [0] = 0xff;
    _orphan_package.to_mac_addr [1] = 0xff;
    _orphan_package.to_mac_addr [2] = 0xff;
    _orphan_package.to_mac_addr [3] = 0xff;
    _orphan_package.to_mac_addr [4] = 0xff;
    _orphan_package.to_mac_addr [5] = 0xff;
    // _orphan_package.PrintOut("AdhocGate::Init()  orphan_package", NULL);
    _orphan_package.PrintOut("AdhocGate::Init()  orphan_package");

}