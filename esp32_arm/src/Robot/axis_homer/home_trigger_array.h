#pragma once

#include "position_trigger.h"
#include "CNC/coordinate/cnc_axis.h"
#include "MyLibs/array/array_base.h"


class HomeTrigger_Array: public ArrayBase{
    public:
        static HomeTrigger_Array& Instance(){
            static HomeTrigger_Array __instance;
            return __instance;
        };
        void Init(PositionTrigger* triggers, int triggers_count){_Init(triggers, triggers_count, sizeof(PositionTrigger));};
        PositionTrigger* GetPositionTrigger(int index){return (PositionTrigger*) _GetItem(index);};
        float GetFiredPosition(char axis_name){return 1.234;};

    private:

};