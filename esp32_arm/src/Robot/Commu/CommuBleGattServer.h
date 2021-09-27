# pragma once

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "BleCallbacks.h"
#include "xtensa/core-macros.h"
#include "CommuDeviceBase.h"

class CommuBleGattServer: public CommuDeviceBase {
    public:
        void Init() override;
        void SpinOnce() override;
        bool HasNewChatting() override;  //Only New message from BleClient.
        char* ReadChatting() override;
        void WriteNotification(const char* notification) override;
        void OutputMessage(std::string message) override;

    protected:

    private:
        BLECharacteristic* pCharChattingroom;
        BLECharacteristic* pCharNotification;
        uint16_t action;

        uint32_t start_timestamp;
        bool command_is_new = false;
        bool is_connected;
        uint8_t* rx_buffer;
        MyBleServerCallbacks myBleServercallbacks = MyBleServerCallbacks(&this->is_connected);
        MyBleCharacteristicCallbacks myBleCharacteristicCallbacks 
                    = MyBleCharacteristicCallbacks(&this->command_is_new, this->rx_buffer);
};
