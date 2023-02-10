

#include "BleCallbacks.h"
#include <esp_log.h>
#include <HardwareSerial.h>


#define LOG_TAG "MyBLE connection."

void MyBleServerCallbacks::onConnect(BLEServer* pServer){
    *is_connected = true;
    Serial.print("\nBLE Client is connected! ");
}

void MyBleServerCallbacks::onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param){
   *is_connected = true;
}

void MyBleServerCallbacks::onDisconnect(BLEServer* pServer){
    *is_connected = false;
    // ESP_LOGW(LOG_TAG, "Client is DIS-connected!");
    Serial.print("\nBLE Client is DIS-connected!");
}

