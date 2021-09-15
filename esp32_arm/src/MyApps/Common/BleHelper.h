#ifndef _BLE_HELPER_H_
#define _BLE_HELPER_H_

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "MyBleServerCallbacks.h"


class BleHelper{
    public:
        void InitBle();
        // char[20] device_name; 

    private:
        BLECharacteristic* pCharRobotAction;
        BLECharacteristic* pCharRobotState;
        // RobotAction action;
        uint16_t action;

        MyBleServerCallbacks* pMyBle;
};



#endif