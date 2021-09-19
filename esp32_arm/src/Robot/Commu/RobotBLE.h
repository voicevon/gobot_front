#ifndef __ROBOT_BLE_H_
#define __ROBOT_BLE_H_

#include "CommuBleGattServer.h"
// #include "CommuDeviceBase.h"

class RobotBle:public CommuBleGattServer {
    public:
        uint8_t GetBuffer(){return 1;};
        // void OutputMessage(string message) override;

    private:
};


#endif