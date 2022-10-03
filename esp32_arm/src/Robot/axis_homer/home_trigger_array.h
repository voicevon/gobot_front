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
        
        float GetFiredPosition(char axis_name){
            float result;
            __bits_flag = 0;
            for(int i=0; i<GetItemsCount();i++){
                PositionTrigger* this_trigger = GetPositionTrigger(i);
                if(this_trigger->IsTriggered()){
                    __bits_flag += 1<< i;
                    if (this_trigger->AxisName == axis_name){
                        result = this_trigger->GetTriggerPosition();
                    }
                }
            }
            return result;
        };

        uint32_t GetStateBitsFlag(){
            return __bits_flag;
        };

    private:
        uint32_t __bits_flag;
};