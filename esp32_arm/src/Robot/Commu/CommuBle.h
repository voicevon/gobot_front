#ifndef __COMMU_ROBOT_BLE_H_
#define __COMMU_ROBOT_BLE_H_

#include "CommuBleGattServer.h"
// #include "CommuDeviceBase.h"

class CommuBle:public CommuBleGattServer {
    public:
        uint8_t GetBuffer(){return 1;};
        // void OutputMessage(string message) override;

    private:
};


#endif