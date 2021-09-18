#ifndef _BLE_SERVER_BASE_H_
#define _BLE_SERVER_BASE_H_

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "BleServerCallbacks.h"
#include "xtensa/core-macros.h"


// #define MAX_GATT_CHARACTORERISTICS 5


class BleServerBase{
    public:
        void Init();
        void SpinOnce();
        bool HasNewChatting();  //Only New message from BleClient.
        char* ReadChatting();
        void WriteNotification(const char* notification);

    protected:
        // void AppendGattChar(uint8_t gattCharId, uint8_t bytesCount);
        // void ReadGattChar(uint8_t gattCharId, uint16_t* buffer,uint8_t length){};
        // void WriteGattChar(uint8_t gattCharId, uint16_t* buffer,uint8_t length){};


    private:
        // BLECharacteristic* gattChars[MAX_GATT_CHARACTORERISTICS];
        BLECharacteristic* pCharChattingroom;
        BLECharacteristic* pCharNotification;

        // BLECharacteristic* pCharRobotAction;
        // BLECharacteristic* pCharRobotState;
        // RobotAction action;
        uint16_t action;

        MyBleServerCallbacks* pMyBle;
        int __State =0; //0==Disconnected,  1==Connecting 2== Connected
        uint32_t __start_at;
        bool command_is_new = false;
};



#endif