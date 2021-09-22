
#include "BleCallbacks.h"
#include <esp_log.h>
#include <HardwareSerial.h>


#define LOG_TAG "MyBLE connection."

void MyBleServerCallbacks::onConnect(BLEServer* pServer){
    // pMyNotifyTask->start();
    *is_connected = true;
    // ESP_LOGW(LOG_TAG, "Client is connected! 11111111111 ");
    Serial.print("\nBLE Client is connected! ");
}

void MyBleServerCallbacks::onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param){
   *is_connected = true;
    // ESP_LOGW(LOG_TAG, "Client is connected!  22222222222222");
//    Serial.print("\nClient is connected! \n\n");
}

void MyBleServerCallbacks::onDisconnect(BLEServer* pServer){
    // pMyNotifyTask->stop();
    *is_connected = false;
    // ESP_LOGW(LOG_TAG, "Client is DIS-connected!");
    Serial.print("\nBLE Client is DIS-connected!");
}

