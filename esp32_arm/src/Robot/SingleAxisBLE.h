#ifndef __SINGLE_AXIS_BLE_H_
#define __SINGLE_AXIS_BLE_H_

#include "MyLibs/BleHelper.h"

class SingleAxisBLE:public BleHelper{
    public:
        SingleAxisBLE();
        int16_t ReadTargetPosAbs();
        void UpdateCurrentPos(int16_t value);

    private:

};

#endif
