#pragma once

#include <BLEServer.h>
#include <HardwareSerial.h>


class MyBleServerCallbacks:public BLEServerCallbacks{
    public:
        MyBleServerCallbacks(bool* is_connected){
            this->is_connected =  is_connected;
        }

        void onConnect(BLEServer* pServer) override;
        void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) override;
        void onDisconnect(BLEServer* pServer) override;

    private:
        bool* is_connected;
};

class MyBleCharacteristicCallbacks: public BLECharacteristicCallbacks{
    public:
        MyBleCharacteristicCallbacks(bool* has_new, uint8_t* rx_buffer){
            this->__has_new = has_new;
            this->__rx_buffer = rx_buffer;
        }
        void onWrite(BLECharacteristic* pCharacteristic){
            Serial.println("Client wrote something.");
            *__has_new = true;
            __rx_buffer =  pCharacteristic->getData();
        }

    private:
        bool* __has_new;
        uint8_t* __rx_buffer;


	// void onRead(BLECharacteristic* pCharacteristic){};
	// void onNotify(BLECharacteristic* pCharacteristic){};
	// void onStatus(BLECharacteristic* pCharacteristic, Status s, uint32_t code){};
};
