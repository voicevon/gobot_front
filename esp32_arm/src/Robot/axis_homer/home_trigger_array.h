#pragma once

#include "position_trigger.h"
#include "CNC/coordinate/cnc_axis.h"
#include "MyLibs/array/array_base.h"
// #include "MyLibs/dictions/diction_base.h"
// #include "MyLibs/common_diction/diction_base.h"


class HomeTrigger_Diction: public ArrayBase{
    public:
        static HomeTrigger_Diction& Instance(){
            static HomeTrigger_Diction __instance;
            return __instance;
        };
        void Init(PositionTrigger* triggers, int triggers_count){_Init(triggers, triggers_count, sizeof(PositionTrigger));};
        PositionTrigger* GetPositionTrigger(int index){return (PositionTrigger*) _GetItem(index);};
        float GetFiredPosition(char axis_name){return 1.234;};

    private:

};