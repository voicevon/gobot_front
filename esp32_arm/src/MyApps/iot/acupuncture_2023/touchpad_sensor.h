#pragma once
#include "stdint.h"
#include "WString.h"
#define SENSOR_HISTORY_QUEUE_SIZE  5


class TouchSensor{
    public:
        enum EnumState{
            NOT_INSTALLED,
            CHANNEL_DIED,
            TOUCHED_ON,
            TOUCHED_OFF,
        };
        void Init(uint8_t channel_id, EnumState state);
        void Review_Sensor_Value(uint8_t new_value);  
        uint8_t GetSensorValue(){return __newest_sensor_value;};
        String GetState();

    private:
        // uint8_t __sensor_id;
        EnumState __state;

        uint8_t __newest_sensor_value;
        uint8_t __untouched_history_values[SENSOR_HISTORY_QUEUE_SIZE];  // TODO: be a queue.
        void __Push_to_HistoryValueWindow(uint8_t new_value);  // newest value at index==0

        

};