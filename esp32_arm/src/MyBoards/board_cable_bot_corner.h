#ifndef _BOARD_CABLE_BOT_CORNER_H_
#define _BOARD_CABLE_BOT_CORNER_H_

#include "boardbase.h"
#include "SimpleFOC/drivers/DCDriverHBridge.h"
#include "SimpleFOC/sensors/Encoder.h"

#define LEDS_COUNT_ON_BOARD 2

class Board_CableBotCorner: public BoardBase<LEDS_COUNT_ON_BOARD>{
    public:
        Board_CableBotCorner();
        // void Init();
        DCDriverHBridge* driver;
        Encoder* encoder;
        // void flash(){this->Flash_AllLeds(1,2,3);};
        // "this->" is important
        //       http://www.cs.technion.ac.il/users/yechiel/CS/FAQs/c++-faq/nondependent-name-lookup-members.html
        
        void flash(){};
        
    private:
};


#endif