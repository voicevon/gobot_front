# pragma once

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "BleCallbacks.h"
#include "xtensa/core-macros.h"
#include "CommuDeviceBase.h"

// #define MAX_GATT_CHARACTORERISTICS 5


class CommuBleGattServer: public CommuDeviceBase {
    public:
        void Init() override;
        void SpinOnce() override;
        bool HasNewChatting() override;  //Only New message from BleClient.
        char* ReadChatting() override;
        void WriteNotification(const char* notification) override;
        void OutputMessage(std::string message) override;

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


        // int __State =0; //0==Disconnected,  1==Connecting 2== Connected
        uint32_t __start_at;
        bool command_is_new = false;
        bool __is_connected;
        bool __is_connecting;
        uint8_t* __rx_buffer;
        MyBleServerCallbacks myBleServercallbacks = MyBleServerCallbacks(&this->__is_connected);
        MyBleCharacteristicCallbacks myBleCharacteristicCallbacks 
                    = MyBleCharacteristicCallbacks(&this->command_is_new, this->__rx_buffer);
};
