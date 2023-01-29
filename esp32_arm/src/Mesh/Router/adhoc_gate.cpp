#include "adhoc_gate.h"


void AdhocGate::onReceived(const uint8_t * mac, const uint8_t *incomingData, int len) {
    if (AdhocHelper::IsSameMacAddr((uint8_t*) mac, __my_mac_addr))
    memcpy(&rx_package,incomingData, len);
    is_new_rx_package = true;
}

void AdhocGate::SpinOnce(){
    // send orphan package always
    
}
void AdhocGate::Init(){
    esp_read_mac(__my_mac_addr, ESP_MAC_WIFI_STA);
    is_new_rx_package = false;
}