#pragma once

#include "position_trigger.h"
#include "CNC/coordinate/cnc_axis.h"
#include "MyLibs/basic/array/array_base.h"


class PositionTrigger_Array: public ArrayBase{
    public:
        static PositionTrigger_Array& Instance(){
            static PositionTrigger_Array __instance;
            return __instance;
        };
        void Init(PositionTrigger* triggers, int triggers_count){_Init(triggers, triggers_count, sizeof(PositionTrigger));};
        PositionTrigger* GetPositionTrigger(int index){return (PositionTrigger*) _GetItem(index);};
        
        uint32_t GetStateBitsFlag(){return __bits_flag;};

        void Test_PositionTriggers(int loops);


    private:
        uint32_t __bits_flag;
};