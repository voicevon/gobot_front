#pragma once
#include "stdint.h"

class TouchPad_Channel{
    public:
        enum EnumState{
            NOT_INSTALLED,
            CHANNEL_DIED,
            CELL_DIED,
            WROKING,
        };
        void Init(uint8_t cell_id, uint8_t channel_id, EnumState state);
        bool Review_Sensor_Value_Whether_Changed();  // return true touched
        void Push_to_HistoryValues(uint8_t new_value);  // newest value at index==0
        void SetStateTo(EnumState new_state);

    private:
        uint8_t __cell_id;
        uint8_t __channel_id;
        uint8_t __untouched_history_values[5];  // TODO: be a queue.
        EnumState __state;
        bool __currently_is_touched = false;
        

};