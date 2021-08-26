#include <BLEServer.h>
#include <esp_log.h>

class MyBleServerCallbacks:public BLEServerCallbacks{
    public:
        bool is_connected = false;


        void onConnect(BLEServer* pServer){
            // pMyNotifyTask->start();
            is_connected = true;
            ESP_LOGD(LOG_TAG, "Client is connected!");
        }
        void onDisconnect(BLEServer* pServer){
            // pMyNotifyTask->stop();
            is_connected = false;
            ESP_LOGD(LOG_TAG, "Client is DIS-connected!");
        }
};