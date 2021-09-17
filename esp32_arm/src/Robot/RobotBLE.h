#ifndef __ROBOT_BLE_H_
#define __ROBOT_BLE_H_

#include "MyLibs/BleServerBase.h"

class RobotBle:public BleServerBase{
    public:
        uint8_t* GetBuffer(){};

    private:
};


#endif