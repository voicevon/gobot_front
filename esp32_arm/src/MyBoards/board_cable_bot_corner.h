#ifndef _BOARD_CABLE_BOT_CORNER_H_
#define _BOARD_CABLE_BOT_CORNER_H_

#include "boardbase.h"
#include "SimpleFOC/drivers/DCDriverHBridge.h"
#include "SimpleFOC/sensors/Encoder.h"


class Board_CableBotCorner:public BoardBase{
    public:
        Board_CableBotCorner();
        void Init();
        DCDriverHBridge* driver;
        Encoder* encoder;
        
    private:
};


#endif