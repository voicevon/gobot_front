#pragma once
#include "stdint.h"
#include "WString.h"
class TouchPad_Channel{
    public:
        enum EnumState{
            NOT_INSTALLED,
            CHANNEL_DIED,
            TOUCHED_ON,
            TOUCHED_OFF,
        };
        void Init(uint8_t channel_id, EnumState state);
        void Push_to_HistoryValues(uint8_t new_value);  // newest value at index==0
        bool Review_Sensor_Value_Whether_Changed();  // return true touched
        void SetStateToDied();  //??  recover by reset system
        String GetPayloadString();
        uint8_t GetSensorValue(){return __capacity_sensor_value;};

    private:
        uint8_t __channel_id;
        uint8_t __untouched_history_values[5];  // TODO: be a queue.
        EnumState __state;
        bool __currently_is_touched = false;
        uint8_t __capacity_sensor_value;
        

};