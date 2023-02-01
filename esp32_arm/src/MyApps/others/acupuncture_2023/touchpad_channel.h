#pragma once
#include "stdint.h"

class TouchPad_Channel{
    public:
        enum EnumState{
            NOT_INSTALLED,
            WROKING,
            CELL_DIED,
            CHANNEL_DIED,
        };
        void Init(uint8_t cell_id, uint8_t channel_id, EnumState state);
        bool Review_Sensor_Value_Whether_Changed();  // return true touched

    private:
        uint8_t __cell_id;
        uint8_t __channel_id;
        uint8_t __untouched_history_values[5];  // TODO: be a queue.
        void __push_to_history_values(uint8_t new_value);  // newest value at index==0
        EnumState __state;
        bool __currently_is_touched = false;
        

};