#if def I_AM_ON_BOARD_WEIGHTING_NODE

void on_esp_now_received(const uint8_t * sender_mac, const uint8_t *incomingData, int len){
    Logger::Debug("on_esp_now_received");
    // gate.onReceived(sender_mac, incomingData, len);
}

void Init_EspNow(){
    WiFi.mode(WIFI_STA);
    // Initilize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Logger::Error("AdhocBase::_Init_EspNow()     Error initializing ESP-NOW");
        Logger::Halt(" esp_now_init()  should be ESP_OK ,   very low level  issue.");
        return;
    }
    ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR));
}

void esp_now_send(uint8_t* pkg){
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

void setup(){
    Init_EspNow();
    
    esp_now_register_recv_cb(on_esp_now_received);
    Logger::Info("Guangda Shuiwu net_gate    setup() is done.....;.");
}

void loop(){

}
#endif