#include "commu_esp_now.h"

void CommuEspNow_Master::OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status){
    Serial.print("\r\n[Info] EspNowMaster::OnDataSent() Last packet sent status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Send Sucess" : "Send Failed");
}

void CommuEspNow_Master::Send(uint8_t slave_node_id, EspNowPayload* data){
    uint8_t slaveMacAddr[6];
    memcpy(&slaveMacAddr, &slave_node_id, 6); 
    //send data
    esp_now_send(0, (uint8_t*)&data, sizeof(data));
    esp_err_t result = esp_now_send(slaveMacAddr, (uint8_t*)&data, sizeof(data));
    if (result == ESP_OK){
        Serial.println("[Info] esp_now_send()   success.");
    }else{
        Serial.println("[Error] esp_now_send() Failed.");
    }
}

void CommuEspNow_Master::InitMaster(const uint8_t* slaveMacAddrs, const uint8_t slaveCount){
    WiFi.mode(WIFI_STA);

    Serial.println(WiFi.macAddress());
    if(esp_now_init() != ESP_OK){
        Serial.println("[Error] esp_now_init(), failed!");
    }
    esp_now_peer_info peerInfo;
    for(int i=0; i< slaveCount; i++){
        const uint8_t* slaveMAC = slaveMacAddrs + 6 * i;
        memcpy(peerInfo.peer_addr, slaveMAC, 6);
        peerInfo.channel = 0;
        peerInfo.encrypt = false;
        if(esp_now_add_peer(&peerInfo) != ESP_OK){
            Serial.println("[Error] esp_now_add_peer(), Failed to add peer");
        }
    }
}

void AddSlave(const uint8_t* slaveMacAddrs){

}






void CommuEspNow_Slave::OnReceived(const uint8_t* macAddr, const uint8_t* data, int len){
    Serial.print("Received from MAC:");
    for (int i=0; i<6; i++){
        Serial.printf("%02X",macAddr[i]);
    }
    int* pMessage = (int*) data;
    Serial.print("   >>> ");
    Serial.print(*pMessage);
}

void CommuEspNow_Slave::InitSlave(){
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK){
        Serial.println("[Error] esp_now_init()  Failed");
    }
    esp_now_register_recv_cb(this->OnReceived);
}