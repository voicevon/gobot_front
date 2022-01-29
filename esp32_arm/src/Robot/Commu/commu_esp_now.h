#pragma once
#include "WiFi.h"
#include "esp_now.h"

struct EspNowPayload{
    int test;
};


class CommuEspNow_Master{
    public:
        CommuEspNow_Master(){};
        void InitMaster(const uint8_t* slaveMacAddr, const uint8_t slaveCount);
        static void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status);  //callback function
        void Send(uint8_t slave_node_id, EspNowPayload* data);
    
    private:
        // uint8_t slaveAddress[6];
};

class CommuEspNow_Slave{
    void InitSlave();
    static void OnReceived(const uint8_t* macAddr, const uint8_t* data, int len);  //callback function

};