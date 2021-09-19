#ifndef __COMMU_SINGLE_AXIS_ROBOT_BLE_H_
#define __COMMU_SINGLE_AXIS_ROBOT_BLE_H_

#include "Robot/Commu/CommuBleGattServer.h"

#define COMMAND_MASK_ENABLE  0b1000000000000000
#define COMMAND_MASK_HOME    0b0000000000000001




class CommuSingleAxisRobotBLE:public CommuBleGattServer{
    public:
        uint8_t GetBuffer(){return 1;};

    //     int16_t GetTargetPositionAbs();
    //     void UpdateCurrentPos(int16_t value);

    // private:
    //     uint16_t CommandMask;
        
};

#endif
