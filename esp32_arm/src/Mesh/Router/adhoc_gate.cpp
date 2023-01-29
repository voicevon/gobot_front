#include "adhoc_gate.h"
#include "MyLibs/basic/logger.h"

void AdhocGate::onReceived(const uint8_t * mac, const uint8_t *incomingData, int len) {
    if (AdhocHelper::IsSameMacAddr((uint8_t*) mac, __my_mac_addr))
    memcpy(&rx_package,incomingData, len);
    is_new_rx_package = true;
}

void AdhocGate::SpinOnce(){
    // Logger::Debug("AdhocRouter::SpinOnce()");
    // Logger::Print("'__my_hop", __my_hop);
    SendOrphan_count_down();
}

void AdhocGate::Init(){
    _Init_EspNow();
    esp_read_mac(__my_mac_addr, ESP_MAC_WIFI_STA);
    is_new_rx_package = false;
    __my_hop = 1;
    __orphan_package.sender_hop= 1;
    __orphan_package.app_source_node_id = 1;
    __orphan_package.app_payload_size = 1;
    __orphan_package.to_mac_addr [0] = 0xff;
    __orphan_package.to_mac_addr [1] = 0xff;
    __orphan_package.to_mac_addr [2] = 0xff;
    __orphan_package.to_mac_addr [3] = 0xff;
    __orphan_package.to_mac_addr [4] = 0xff;
    __orphan_package.to_mac_addr [5] = 0xff;
    __orphan_package.PrintOut("AdhocGate::Init()  orphan_package");

}