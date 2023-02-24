#include "position_trigger_array.h"

void PositionTrigger_Array::Test_PositionTriggers(int loops){
    uint32_t flags = 0;
    uint32_t last_flags = 999;
    int count =0;
    while (count < loops){
        flags = 0;
        for (int index=0; index < this->GetItemsCount(); index++){
            if (this->GetPositionTrigger(index)->IsFired()){
                flags += 1<<index;
            }
        }
        if (flags != last_flags){
            Serial.print("Trigger is channged   0:normal,   1:triggered \t\t");
            Serial.print("Counter= ");
            Serial.print(count);
            Serial.print("  ");
            Serial.println(flags, BIN);

            last_flags = flags;
            count++;
        }
    }
}