#include <BLEServer.h>
class MyBleServerCallbacks:public BLEServerCallbacks{
    public:
        bool is_connected = false;

        void onConnect(BLEServer* pServer) override;
        void onDisconnect(BLEServer* pServer) override;
        void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) override;
};