#ifndef _BLE_HELPER_H_
#define _BLE_HELPER_H_

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "BleServerCallbacks.h"

#define MAX_GATT_CHARACTORERISTICS 5


class BleHelper{
    public:
        void InitBle();

    protected:
        void AppendGattChar(uint8_t gattCharId, uint8_t bytesCount);
        void ReadGattChar(uint8_t gattCharId, uint16_t* buffer,uint8_t length){};
        void WriteGattChar(uint8_t gattCharId, uint16_t* buffer,uint8_t length){};


    private:
        BLECharacteristic* gattChars[MAX_GATT_CHARACTORERISTICS];

        BLECharacteristic* pCharRobotAction;
        BLECharacteristic* pCharRobotState;
        // RobotAction action;
        uint16_t action;

        MyBleServerCallbacks* pMyBle;

};



#endif