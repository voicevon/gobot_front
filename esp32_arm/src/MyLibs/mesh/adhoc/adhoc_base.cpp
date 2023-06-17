#include "adhoc_base.h"
#include "von/utility/logger.h"


void AdhocBase::_Init_EspNow(){
    WiFi.mode(WIFI_STA);
    // Initilize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Logger::Error("AdhocBase::_Init_EspNow()     Error initializing ESP-NOW");
        Logger::Halt(" esp_now_init()  should be ESP_OK ,   very low level  issue.");
        return;
    }
    ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR));
}

void AdhocBase::_Send(AdhocPackage* pkg){
    _time_count_up = 0;
    // Emulates a broadcast
    // Broadcast a message to every device in range
    uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    esp_now_peer_info_t peerInfo = {};
    memcpy(&peerInfo.peer_addr, broadcastAddress, 6);
    if (!esp_now_is_peer_exist(broadcastAddress)){
        esp_now_add_peer(&peerInfo);
    }
    // Send message
    const uint8_t* message = (const uint8_t*) pkg;
    uint8_t length = sizeof(AdhocPackage);
    esp_err_t result = esp_now_send(broadcastAddress, message, length);

    // Print results to serial monitor
    if (result == ESP_OK){
        Serial.print("O");
    } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
        Serial.println("ESP-NOW not Init.");
    } else if (result == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Invalid Argument");
    } else if (result == ESP_ERR_ESPNOW_INTERNAL)  {
        Serial.println("Internal Error");
    } else if (result == ESP_ERR_ESPNOW_NO_MEM)  {
        Serial.println("ESP_ERR_ESPNOW_NO_MEM");
    } else if (result == ESP_ERR_ESPNOW_NOT_FOUND)  {
        Serial.println("Peer not found.");
    } else {
        Serial.println("Unknown error");
    }
}


// void AdhocBase::_Send_Ophan(){
//     _Send(&__orphan_package);
// }

void AdhocBase::Broadcast_Orphan_count_down(){
    // Logger::Debug("AdhocBase::Broadcast_Orphan_count_down()");
    // Logger::Print("'__my_hop", __my_hop);
    _time_count_up++;
    if (_time_count_up >= 6999999){
        // Logger::Debug("AdhocBase::Broadcast_Orphan_count_down() " );
        // Logger::Print("__my_hop", __my_hop);

        _orphan_package.sender_hop = _my_hop;        
        // __orphan_package.PrintOut("AdhocBase::Broadcast_Orphan_count_down()  sending orphan...");
        _Send(&_orphan_package);
        return;
    }
}