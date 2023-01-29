#include "adhoc_sniffer.h"


void AdhocSniffer::onReceived(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&rx_package,incomingData, len);
    is_new_rx_package = true;
}

void AdhocSniffer::SpinOnce() {

}

void AdhocSniffer::Init() {
    is_new_rx_package = false;
}

